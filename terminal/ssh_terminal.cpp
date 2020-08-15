//
// Created by min on 2020/8/7.
//

#include "ssh_terminal.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

SSHTerminal::SSHTerminal(const SSHSettings &settings, QWidget *parent) : BaseTerminal(parent) {
    settings_ = settings;
    // Here we start an empty pty.
    this->startTerminalTeletype();
}

SSHTerminal::~SSHTerminal() {
}

void SSHTerminal::connect() {
    int rc, i;
    struct sockaddr_in sin;
    const char *fingerprint;

    rc = libssh2_init(0);
    if(rc != 0) {
        fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
        return;
    }

    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(settings_.host.c_str());
    sin.sin_port = htons(22);
    if(::connect(sock_, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0) {
        fprintf(stderr, "failed to connect!\n");
        return;
    }

    /* Create a session instance and start it up. This will trade welcome
     * banners, exchange keys, and setup crypto, compression, and MAC layers
     */
    session_ = libssh2_session_init();
    if(libssh2_session_handshake(session_, sock_)) {
        fprintf(stderr, "Failure establishing SSH session\n");
        return;
    }

    /* At this point we havn't authenticated. The first thing to do is check
     * the hostkey's fingerprint against our known hosts Your app may have it
     * hard coded, may go to a file, may present it to the user, that's your
     * call
     */
    fingerprint = libssh2_hostkey_hash(session_, LIBSSH2_HOSTKEY_HASH_SHA1);
    fprintf(stderr, "Fingerprint: ");
    for(i = 0; i < 20; i++) {
        fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
    }
    fprintf(stderr, "\n");

    if(libssh2_userauth_password(session_, settings_.user.c_str(), settings_.passwd.c_str())) {
        fprintf(stderr, "\tAuthentication by password failed!\n");
        return;
    } else {
        fprintf(stderr, "\tAuthentication by password succeeded.\n");
    }

    /* Request a shell */
    channel_ = libssh2_channel_open_session(session_);
    if(!channel_) {
        fprintf(stderr, "Unable to open a session\n");
        return;
    }

    /* Some environment variables may be set,
     * It's up to the server which ones it'll allow though
     */
    libssh2_channel_setenv(channel_, "TERM", "xterm-256color");

    /* Request a terminal with 'vanilla' terminal emulation
     * See /etc/termcap for more options
     */
    if(libssh2_channel_request_pty(channel_, "xterm-256color")) {
        fprintf(stderr, "Failed requesting pty\n");
        return;
    }

//    libssh2_channel_request_pty_size(channel_, 120, 24);

    /* Open a SHELL on that pty */
    if(libssh2_channel_shell(channel_)) {
        fprintf(stderr, "Unable to request shell on allocated pty\n");
        return;
    }

    // 把在终端的输入传给 ssh
    QObject::connect(this, &QTermWidget::sendData, [this](const char *data, int size) {
        if(connect_) {
            libssh2_channel_write(channel_, data, size);
        }
    });

    // 起个线程，循环读 ssh 返回的数据，写到终端
    sshReadThread_ = std::thread(std::mem_fn(&SSHTerminal::threadLoop), this);
    fprintf(stderr, "connect ssh success\n");
    connect_ = true;
}

void SSHTerminal::disconnect() {
    if (!connect_) {
        return;
    }
    connect_ = false;
    sshReadThread_.join();
    if(channel_) {
        libssh2_channel_free(channel_);
        channel_ = NULL;
    }
    libssh2_session_disconnect(session_,"Normal Shutdown, Thank you for playing");
    libssh2_session_free(session_);
    ::close(sock_);
    fprintf(stderr, "all done!\n");
    libssh2_exit();
}

void SSHTerminal::threadLoop() {
    LIBSSH2_POLLFD *fds = nullptr;
    int nfds = 1;
    char buffer;

    fds = static_cast<LIBSSH2_POLLFD *>(malloc(sizeof(LIBSSH2_POLLFD)));
    if (fds == nullptr) {
        fprintf(stderr, "malloc LIBSSH2_POLLFD error\n");
        return;
    }

    fds[0].type = LIBSSH2_POLLFD_CHANNEL;
    fds[0].fd.channel = channel_;
    fds[0].events = LIBSSH2_POLLFD_POLLIN;
    fds[0].revents = LIBSSH2_POLLFD_POLLIN;

    while (connect_) {
        if (libssh2_poll(fds, nfds, 1000) > 0) {
            libssh2_channel_read(channel_, &buffer, 1);
            write(this->getPtySlaveFd(), &buffer, 1);
            fflush(stdout);
        }
    }
}

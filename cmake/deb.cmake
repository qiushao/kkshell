set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "qiushao")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "a Linux terminal manager")
set(CPACK_DEBIAN_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt5-default, libqt5serialport5, libutf8proc, expect, zssh")
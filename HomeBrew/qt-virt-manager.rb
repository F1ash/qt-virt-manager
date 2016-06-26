class QtVirtManager < Formula
  desc "Qt App for managing virtual machines"
  homepage "http://f1ash.github.io/qt-virt-manager/"
  url "https://fedorapeople.org/qt-virt-manager/qt-virt-manager-0.??.??.tar.gz"
  sha256 "???"

  depends_on "gobject-introspection" => :build
  depends_on "intltool" => :build
  depends_on "libtool" => :build
  depends_on "pkg-config" => :build

  depends_on "qt5"
  depends_on "qtermwidget"
  depends_on "glib"
  depends_on "x11vnc"
  depends_on "hicolor-icon-theme"
  depends_on "libvirt"
  depends_on "libvirt-glib"
  depends_on "spice-protocol"
  depends_on "usbredir"
  depends_on "gtk+3"
  depends_on :x11


  def install
    system "cmake", "-DBUILD_QT_VERSION=5",
                    "-DWITH_LIBCACARD=0",
                    "-DBUILD_TYPE=Release",
                    "-DUSE_SPICE_AUDIO=1",
                    ".."
    system "make",  "all"
    system "make",  "install"
  end

  def post_install
    # manual schema compile step
    system "#{Formula["glib"].opt_bin}/glib-compile-schemas", "#{HOMEBREW_PREFIX}/share/glib-2.0/schemas"
    # manual icon cache update step
    system "#{Formula["gtk+3"].opt_bin}/gtk3-update-icon-cache", "#{HOMEBREW_PREFIX}/share/icons/hicolor"
  end
end

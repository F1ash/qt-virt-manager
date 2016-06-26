class QTermWidget5 < Formula
  desc "A terminal emulator widget for Qt 5"
  homepage "https://github.com/lxde/qtermwidget"
  url "https://f1ash.fedorapeople.org/qt-virt-manager/QTermWidget5/qt5-qtermwidget.zip"
  sha256 "0a371c3d7ff2cc4cd33526b34733d24380dc5f51dbafb6fa7ad0e7f49568d2b5"

  depends_on "libtool" => :build
  depends_on "pkg-config" => :build

  depends_on "qt5"

  def install
    system "mkdir", "-p", "build"
    system "cd",    "build"
    system "cmake", ".."
    system "make"
    system "make",  "install"
  end
end

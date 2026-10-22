# Maintainer: right9code <https://github.com/right9code>
pkgname=clean-spice
pkgver=1.0.0
pkgrel=1
pkgdesc="Minimal zero-header frameless SPICE client for Wayland tiling window managers"
arch=('x86_64')
url="https://github.com/right9code/clean-spice"
license=('MIT')
depends=('gtk3' 'spice-gtk')
makedepends=('gcc' 'make' 'pkgconf')
source=("$pkgname-$pkgver.tar.gz::$url/archive/refs/tags/v$pkgver.tar.gz")
sha256sums=('SKIP')

build() {
    cd "$pkgname-$pkgver"
    make
}

package() {
    cd "$pkgname-$pkgver"
    make DESTDIR="$pkgdir" PREFIX=/usr install
    install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}

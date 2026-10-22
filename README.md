# clean-spice

**clean-spice** is an ultra-minimal, frameless, zero-header SPICE client designed specifically for **Wayland tiling window managers** (Hyprland, Sway, River, Niri, DWL).

---

## The Problem It Solves

When running Windows or Linux virtual machines via QEMU/KVM on Wayland tiling compositors:
- **`virt-viewer`** hardcodes GTK3 Client-Side Decorations (CSD). Its bulky headerbar, title text, and minimize/maximize/close buttons are drawn inside the client canvas, cluttering your screen and ignoring tiling window rules.
- **`spicy`** renders a full desktop menu bar (`File`, `Edit`, `View`...) plus a secondary action toolbar.
- **`virt-viewer --kiosk`** removes headers but locks cursor focus, preventing window management or releasing the pointer.

**clean-spice** connects directly to the QEMU SPICE display and provides **pure framebuffer output with zero headers, zero toolbars, and zero buttons**. Your window manager handles tiling, borders, floating, and closing natively.

---

## Features

- **Zero Window Chrome**: No titlebars, no CSD headerbars, no toolbars, no menus.
- **Pure Wayland Native**: Runs directly on Wayland (`GDK_BACKEND=wayland`).
- **Dynamic Guest Resizing**: Automatically updates guest display resolution as you resize or tile windows (`resize-guest=TRUE`).
- **Bidirectional Clipboard**: Seamless copy/paste between host and guest (`auto-clipboard=TRUE`).
- **USB Redirection Support**: Automatically forwards USB devices when requested.
- **Ultra Lightweight**: Single 50-line C binary compiled directly against `spice-client-gtk-3.0`.

---

## Installation

### Arch Linux (via PKGBUILD)

```bash
git clone https://github.com/right9code/clean-spice.git
cd clean-spice
makepkg -si
```

### Manual Compilation & Install

**Dependencies:**
- Arch: `gtk3`, `spice-gtk`, `gcc`, `make`, `pkgconf`
- Debian/Ubuntu: `libgtk-3-dev`, `libspice-client-gtk-3.0-dev`, `gcc`, `make`, `pkg-config`
- Fedora: `gtk3-devel`, `spice-gtk3-devel`, `gcc`, `make`

```bash
git clone https://github.com/right9code/clean-spice.git
cd clean-spice
make
sudo make install
```

---

## Usage

```bash
# Connect to default local QEMU SPICE instance
clean-spice

# Or specify a custom SPICE URI
clean-spice spice://127.0.0.1:5900
clean-spice spice+unix:///run/user/1000/vm.sock
```

---

## Recommended Hyprland Integration

Add window rules to `~/.config/hypr/hyprland.conf` or your Omarchy config:

```ini
# Clean tiling without unnecessary window borders
windowrulev2 = noborder, class:^(clean-spice)$
windowrulev2 = norounding, class:^(clean-spice)$
```

### Example Hotkey Toggle Script

```bash
#!/usr/bin/env bash
VM_NAME="Windows11"
STATE=$(virsh --connect qemu:///system domstate "$VM_NAME" 2>/dev/null)
VIEWER_PID=$(pgrep -f "clean-spice")

if [ "$STATE" = "running" ] && [ -n "$VIEWER_PID" ]; then
    # Window open -> freeze CPU and hide
    pkill -f "clean-spice"
    virsh --connect qemu:///system suspend "$VM_NAME"
elif [ "$STATE" = "paused" ]; then
    # Unpause and restore display
    virsh --connect qemu:///system resume "$VM_NAME"
    clean-spice &
else
    # Boot VM if shut off and show
    virsh --connect qemu:///system start "$VM_NAME"
    sleep 2
    clean-spice &
fi
```

---

## Author & License

- **Author:** [right9code](https://github.com/right9code)
- **License:** Non-Commercial License (Personal & Educational Use Only). **Commercial use of any kind is strictly prohibited.** See [LICENSE](LICENSE) for details.

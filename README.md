# Frangitron Raspberry

Raspberry Pi part of the Frangitron

## Launch at startup

Add to the end of ~/.bashrc

```bash
# FRANGITRON
if ! [ "$SSH_CONNECTION" -a "$SSH_TTY" == $(tty) ]; then
    ./frangitron/frangitron --platform linuxfb
fi
```

_This will avoid program launch when connecting through ssh to pull and build_

## Dependencies

| Ubuntu 18 | Raspbian Stretch |
| --- | --- |
|   | `qt5-default` |
| `libsndfile`  | `libsndfile1-dev` |
|   | ` libasound2-dev` |

## Tutorials

- https://arlencox.wordpress.com/2010/11/21/building-your-own-synthesizer-part-1-getting-started/
- http://www.martin-finke.de/blog/
- https://forum.cockos.com/showthread.php?t=210390
- https://lemariva.com/blog/2018/07/raspberry-pi-preempt-rt-patching-tutorial-for-kernel-4-14-y

## Notes

### Raspberry Pi 3

#### Boot

- https://raspberrypi.stackexchange.com/questions/57128/how-to-boot-into-own-python-script-gui-only
- https://yingtongli.me/blog/2016/12/21/splash.html
- https://scribles.net/silent-boot-up-on-raspbian-stretch/
- https://scribles.net/customizing-boot-up-screen-on-raspberry-pi/

#### 7" LCD touchscreen brightness

```bash
/sys/class/backlight/rpi_backlight/brightness
```

#### Force turbo

In `/boot/config.txt` set `force_turbo=1`

### Rt Scheduling

Edit limits `sudo nano /etc/security/limits.conf`

```text
@audio -    rtprio    90
@audio -    memlock   unlimited
pi     -    rtprio    90
pi     -    memlock   unlimited
```

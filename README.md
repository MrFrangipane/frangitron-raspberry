# Frangitron Raspberry

Raspberry Pi part of the Frangitron

## Launch

On a headless Pi

```bash
nice -10 ./build_rtaudiortmidi/project /path/to/configuration.json -platform linuxfb
```

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
for i in `seq 0 255`;do echo $i > /sys/class/backlight/rpi_backlight/brightness;echo $i;sleep 0.1;done
```

#### Force turbo

In `/boot/config.txt` set `force_turbo=1`

### Pisound driver + Realtime Kernel

- Download and install from https://github.com/BlokasLabs/rpi-kernel-rt
- Edit limits `sudo nano /etc/security/limits.conf`

```text
@audio -    rtprio    90
@audio -    memlock   unlimited
@audio -    nide      -10
pi     -    rtprio    90
pi     -    memlock   unlimited
pi     -    nide      -10
```

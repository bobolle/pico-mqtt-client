# pico-mqtt-client

lazy mqtt client for pico-w.

## building

```shell
git clone git@github.com:bobolle/pico-mqtt-client.git
git submodule update --init --recursive

# edit CMakeLists.txt

cmake -B build
make -C build

# start pico in bootsel mode & mount the .uf2
sudo mount /path/to/pico /path/to/mountpoint
sudo cp .uf2 /path/to/mountpoint
sudo umount /path/to/mountpoint
```

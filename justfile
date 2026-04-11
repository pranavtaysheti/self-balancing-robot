port := "/dev/ttyACM0"
src := "./code/"
board := "rp2040:rp2040:rpipicow"

update-compile-commands:
    arduino-cli compile \
    --fqbn rp2040:rp2040:rpipicow \
    --build-path build \
    --only-compilation-database
    {{ src }}

build:
    arduino-cli compile --fqbn {{ board }} {{ src }}

upload:
    arduino-cli upload -p {{ port }} {{ board }} {{ src }}

monitor:
    arduino-cli monitor -p /dev/ttyACM0 -c baudrate=115200

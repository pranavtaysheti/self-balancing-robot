port := "/dev/ttyACM0"
src := "./code/"
board := "rp2040:rp2040:rpipicow"
cc_file := "compile_commands.json"
baud_rate := "115200"

compdb:
    arduino-cli compile \
    --fqbn {{ board }} \
    --build-path build \
    --only-compilation-database \
    {{ src }}
    [ -e {{ cc_file }} ] && rm -- {{ cc_file }}
    ln -s ./build/compile_commands.json .

build:
    arduino-cli compile --fqbn {{ board }} {{ src }}

upload:
    arduino-cli upload -p {{ port }} {{ board }} {{ src }}

monitor:
    arduino-cli monitor -p {{ port }} -c baudrate={{ baud_rate }}

build-all: compdb build

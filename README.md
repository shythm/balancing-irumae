# 이루매 밸런싱 인형 로봇

ATmega128을 이용한 이루매 밸런싱 인형 로봇

## Miscellaneous

- `upload.sh`

  - 프로젝트를 빌드하고 `avrdude` 명령어를 이용하여 업로드까지 하는 자동화 쉘 스크립트이다.

  ```sh
  #!/bin/bash
  set -e
  export PROJECT=balancing-irumae
  export PROGRAMMER=stk500
  export PORT=/dev/tty.SLAB_USBtoUART

  # Build the project
  echo "Building the project..."
  cd build
  make

  # Upload the project
  avrdude -c $PROGRAMMER -P $PORT -p m128 -U flash:w:$PROJECT.hex
  ```

- `.vscode/c_cpp_properties.json`

  - vscode에서 include path를 지정하여 C/C++ IntelliSense 기능을 활용할 수 있다.

  ```json
  {
    "configurations": [
      {
        "name": "Mac",
        "includePath": [
          "${workspaceFolder}/**",
          "/opt/homebrew/Cellar/avr-gcc@9/9.4.0/avr/include"
        ],
        "compilerPath": "/opt/homebrew/bin/avr-gcc",
        "cStandard": "c99",
        "cppStandard": "c++17",
        "intelliSenseMode": "macos-gcc-arm64"
      }
    ],
    "version": 4
  }
  ```

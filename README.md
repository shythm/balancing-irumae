# 이루매 밸런싱 인형 로봇

ATmega128을 이용한 이루매 밸런싱 인형 로봇

## Develop Log

- 2023.11.18. MPU6050와 I2C 인터페이스로 통신하여 3차원 상의 가속도, 각속도 값을 가져오는데 성공했다.
  - [i2c.h](./src/i2c.h), [mpu6050.c](./src/mpu6050.c), [mpu6050.h](./src/mpu6050.h)
- 2023.11.17. 디버깅 및 로깅을 위한 컴퓨터와의 UART 통신에 성공했다. 서식 문자도 지정할 수 있도록 코드 작성함.
  - [serial.c](./src/serial.c), [serial.h](./src/serial.h)
- 2023.11.16. JKIT-128-1 개발 보드에 대한 개발 환경을 구축했다.
  - [Makefile](./build/Makefile)

## Trial and Error

- MPU6050 레지스터를 읽고 나면 Start I2C Condition에서 블락되는 문제 발생 및 해결.
  - 상황 및 문제
    - 0x3B(ACCEL_XOUT_H) 레지스터를 지정하고 이후 6바이트를 연속적으로 읽는 코드를 작성함.
    - 이를 통해 다음에 있는 Y축, Z축에 대한 가속도를 연속적으로 읽으려 했었음.
    - 이렇게 한번 읽고 나서, 다음에 또 읽을 때 Start I2C Condition에서 블락이 되는 문제 발생.
  - 디버깅
    - I2C 통신하는 각 시퀀스 곳곳에 디버깅 코드(uart 출력 코드)를 넣어 어디서 블락이 되는지 확인.
    - MPU6050 데이터 시트 및 레지스터 맵 확인
    - 예제 코드를 살펴보며 나의 코드와 무엇이 다른지 확인 -> 다른 코드들은 X, Y, Z축 하나씩 읽는데 나만 한번에 읽음.
  - 해결
    - MPU6050 Datasheet 9.3 I2C Communications Protocol 참조.
    - Single-Byte Read Sequence와 Burst Read Sequence 경우가 존재하는데, 전자는 1바이트 하나만 읽을 때이고, 후자는 2바이트를 연속으로 읽을 때의 시퀀스를 설명하고 있음.
      ```
      MASTER <S> <AD+W>       <RA>       <S> <AD+R>              <ACK>        <NACK> <P>
      SLAVE             <ACK>      <ACK>            <ACK> <DATA>       <DATA>
      ```
    - **애초에 1바이트 또는 2바이트만 한 시퀀스에 읽을 수 있는 것임.**
    - 2바이트로 구성된 레지스터는 위의 시퀀스대로만 읽기 작업을 수행하니 원할히 값을 가져올 수 있었음.

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

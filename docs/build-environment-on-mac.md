# AVR 마이크로프로세서 Mac 개발 환경 구축

1. CP210x USB to UART Bridge VCP Drivers 설치

    [CP210x USB to UART Bridge VCP Drivers - Silicon Labs](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads)에서 "CP210x VCP Mac OSX Driver" 다운로드 및 설치
    
2. 보드를 연결한 후 정상적으로 인식되는지 확인
    
    ```bash
    % ls -l /dev/tty.SLAB_USBtoUART
    ```
    
3. AVR GNU Compiler 설치
    
    https://github.com/osx-cross/homebrew-avr/
    
    ```bash
    % xcode-select --install
    % brew tap osx-cross/avr
    % brew install avr-gcc
    ```
    
4. avrdude 설치
    
    https://github.com/avrdudes/avrdude

    ```bash
    % brew install avrdude
    ```

## JKIT-128-1 Template

- `Makefile`
    ```makefile
    ## Project Name
    PROJECT = jkit-128-template
    TARGET = $(PROJECT).elf

    ## General Flags
    MCU = atmega128
    CC = avr-gcc
    CPP = avr-g++

    ## Options common to compile, link and assembly rules
    COMMON = -mmcu=$(MCU)

    ## Compile options common for all C compilation units.
    ## -Wall: Enable all warnings
    ## -std=gnu99: Use the GNU C99 extensions
    ## -Os: Optimize for size
    ## -fpack-struct: Pack structure members together without holes(data structure alignment)
    CFLAGS = $(COMMON)
    CFLAGS += -Wall -std=gnu99 -Os -fpack-struct
    ## For print float
    CFLAGS += -Wl,-u,vfprintf -lprintf_flt -lm

    ## Linker flags
    ## -Wl, -Map: Create a link map
    LDFLAGS = $(COMMON)
    LDFLAGS += -Wl,-u,vfprintf,-Map=$(PROJECT).map

    ## Intel Hex file production flags
    HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

    HEX_EEPROM_FLAGS = -j .eeprom
    HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
    HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

    ## Libraries
    LIBS = -lm -lprintf_flt
    LIBDIRS = 
    INCLUDES = 

    ## Objects
    OBJECTS = main.o
    LINKONLYOBJECTS =

    ## Build
    all: $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss

    ## Compile
    %.o: %.c
        $(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

    ## Link
    $(TARGET): $(OBJECTS)
        $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $@

    ## Create flash image
    %.hex: $(TARGET)
        avr-objcopy -O ihex $(HEX_FLASH_FLAGS) $< $@

    ## Create eeprom image
    %.eep: $(TARGET)
        -avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

    ## Create extended listing file from ELF output file
    %.lss: $(TARGET)
        avr-objdump -h -S $< > $@

    ## Clean target
    .PHONY: clean
    clean:
        -rm -rf $(OBJECTS) $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map
    ```
    - 새로운 오브젝트 파일 생성이 필요한 경우 `OBJECTS`에 추가
    - 바이너리 파일 이름은 `PROJECT`에 지정
    
- `upload.sh`
    
    ```bash
    avrdude -c stk500 -P /dev/tty.SLAB_USBtoUART -p m128 -U flash:w:jkit-128-template.hex
    ```
    
    - `make` 명령 수행 후 생성된 바이너리 파일을 업로드하는 명령어

- `main.c`
    
    ```c
    #include <avr/io.h>
    #include <avr/iom128.h>
    
    int main() {
        DDRC = 0xFF;
        DDRG = 0x0F;
        PORTC = 0xFF;
        PORTG = 0x0F;
    }
    ```
    
- `.vscode/c_cpp_properties.json`
    
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

# FL-DCD-7224-4 数字光源控制器 固件

## 项目概述

基于 STM32G030C8T6 的 4 通道 LED 光源控制器固件，实现追光者 FL-DCD-7224-4 使用说明书的全部功能。

## 硬件资源

| 外设 | 引脚 | 功能 |
|------|------|------|
| TIM1 CH1 | PA8 | PWM1 输出 |
| TIM1 CH2 | PB3 | PWM2 输出 |
| TIM1 CH3 | PB4 | PWM3 输出 |
| TIM1 CH4 | PB5 | PWM4 输出 |
| USART1 TX | PA9 | RS232 发送 |
| USART1 RX | PA10 | RS232 接收 |
| ADC1 IN0 | PA0 | 过流检测 |
| GPIO | PA4-PA7 | 触发输入 TR1-TR4 |
| GPIO | PB6 | 按键 CH (通道切换) |
| GPIO | PB7 | 按键 - (减) |
| GPIO | PB8 | 按键 + (加) |
| GPIO | PB10-PB11 | CH455G 显示驱动 |
| GPIO | PB12-PB14 | LED 指示灯 |

## 与说明书的不同之处

1. **参数保存延迟**：按键或 RS232 修改参数后 **30 秒**才写入 Flash，避免频繁写入损耗
2. **PWM 频率可选**：通过宏定义选择 64K / 250K / 500K
3. **分辨率最大化**：64kHz → 1000 级 (0-999)，250kHz → 256 级 (0-255)，500kHz → 128 级 (0-127)

## PWM 频率选择

在 `Core/Inc/main.h` 中，取消注释 **其中一个** 宏定义：

```c
#define PWM_FREQ_64K      // 64kHz, 分辨率 0-999 (推荐)
// #define PWM_FREQ_250K   // 250kHz, 分辨率 0-255
// #define PWM_FREQ_500K   // 500kHz, 分辨率 0-127
```

### 计算公式

```
PWM频率 = HCLK / (Prescaler + 1) / (ARR + 1)

64kHz:  64MHz / (0+1) / (999+1)  = 64kHz,   分辨率 = 1000级
250kHz: 64MHz / (0+1) / (255+1)  = 250kHz,  分辨率 = 256级
500kHz: 64MHz / (0+1) / (127+1)  = 500kHz,  分辨率 = 128级
```

## 功能实现

### 工作模式

| 模式 | 指示灯 | 功能 |
|------|--------|------|
| 常亮模式 (0) | B 亮 | LED 常亮，亮度 0-999 可调 |
| 触发模式1 (1) | B+S 亮 | 触发高电平 → LED 亮 |
| 触发模式2 (2) | B+P 亮 | 触发上升沿 → LED 亮设定脉宽 |

### 按键操作

- **CH 短按**: 切换通道 1→2→3→4→模式选择→1
- **CH 长按** (模式2): 进入/退出脉宽设置
- **+/- 短按**: 亮度 ±1
- **+/- 长按**: 亮度快速 ±1
- **模式选择时 +/-**: 切换工作模式

### RS232 通信协议

波特率 115200, 8N1, ASCII 协议, 以 `#` 结尾。

| 指令 | 格式 | 示例 | 说明 |
|------|------|------|------|
| 设置单通道亮度 | `SLV<ch><val>#` | `SLV1130#` | 设置通道1亮度130 |
| 设置全部通道亮度 | `SLV0<v1><v2><v3><v4>#` | `SLV0130140150160#` | |
| 读取通道亮度 | `RLV<ch>#` | `RLV1#` | 返回 `RLV1XXX#` |
| 读取全部亮度 | `RLV0#` | | 返回 `RLV0XXXXXXXXXXXX#` |
| 设置工作模式 | `SM<mode>#` | `SM1#` | 设置触发模式1 |
| 读取工作模式 | `RM#` | | 返回 `RM<mode>#` |
| 设置脉宽 | `SPW<ch><val>#` | `SPW1120#` | 模式2下设置脉宽 |
| 读取脉宽 | `RPW<ch>#` | `RPW1#` | 返回 `RPW1XXX#` |

## 编译与烧录

### 前置条件

1. 安装 ARM 工具链: `arm-none-eabi-gcc`
2. 下载 [STM32CubeG0 HAL 库](https://github.com/STMicroelectronics/STM32CubeG0)
3. 将 HAL 库放到 `Drivers/` 目录

### 编译

```bash
make
```

### 烧录 (OpenOCD + ST-Link)

```bash
make flash
```

### 查看固件大小

```bash
make size
```

## 目录结构

```
stm32_project/
├── Core/
│   ├── Inc/
│   │   ├── main.h              # 主头文件 (PWM频率选择在这里)
│   │   └── stm32g0xx_it.h      # 中断头文件
│   └── Src/
│       ├── main.c              # 主入口
│       ├── tim.c               # TIM1 PWM 配置
│       ├── usart.c             # RS232 串口通信
│       ├── gpio.c              # GPIO 初始化
│       ├── ch455g.c            # CH455G 数码管驱动
│       ├── buttons.c           # 按键扫描
│       ├── params.c            # 参数存储 (30秒延迟保存)
│       ├── trigger.c           # 触发输入处理
│       ├── overcurrent.c       # 过流保护
│       └── stm32g0xx_it.c      # 中断处理
├── Drivers/                    # STM32CubeG0 HAL (需自行下载)
├── Makefile
├── STM32G030C8Tx_FLASH.ld      # 链接脚本
├── FL-DCD-7224-4.ioc           # CubeMX 配置说明
├── manual.pdf                  # 使用说明书
├── schematic1.pdf              # 原理图1 (MCU + 电源 + 驱动)
└── schematic2.pdf              # 原理图2 (显示 + 按键 + RS232)
```

## 注意事项

1. 过流保护阈值 (`OC_THRESHOLD`) 需根据实际硬件测量调整
2. CH455G 7段显示码表 (`seg_font[]`) 需对照实际显示效果微调
3. 参数存储在 Flash 最后 2KB (0x0800F800)，使用 magic number 验证
4. 64kHz 时 TIM1 ARR=999，实现 1000 级亮度调节，与说明书一致

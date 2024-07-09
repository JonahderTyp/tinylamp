# Byte and Bit Diagram

## Byte Structure

### Command Byte (1 byte)
- **Bits**: 8 bits (b7 to b0)
- **MSB (Most Significant Bit)**: b7
  - **If MSB (b7) = 1**: Function Byte
  - **If MSB (b7) = 0**: Command Byte

### Data Bytes (3 bytes)
- **Data Byte 1**: 8 bits (b7 to b0)
- **Data Byte 2**: 8 bits (b7 to b0)
- **Data Byte 3**: 8 bits (b7 to b0)

## Byte Breakdown

### Command Byte Breakdown
- **b7**: 0 (Indicates Command Byte)
- **b6 to b0**: Command details

### Function Byte Breakdown
- **b7**: 1 (Indicates Function Byte)
- **b6 to b0**: Function details

## Data Byte Usage

### If Command Byte
- **Data Byte 1**: Color
  - **Bits**: 8 bits (b7 to b0)
- **Data Byte 2**: Brightness
  - **Bits**: 8 bits (b7 to b0)
- **Data Byte 3**: Reserved
  - **Bits**: 8 bits (b7 to b0)

### If Function Byte
- **Data Byte 1**: Settings
  - **Bits**: 8 bits (b7 to b0)
- **Data Byte 2**: Alert
  - **Bits**: 8 bits (b7 to b0)
- **Data Byte 3**: Reserved
  - **Bits**: 8 bits (b7 to b0)

## Diagram


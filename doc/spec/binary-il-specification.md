# LMNtal binary IL specification

LMNtal binary IL is variable-length binary representation of LMNtal IL.

## Il format

All IL is started with 1 byte opcode and followed by operands.

### Opcodes

| Opcode | Name      | Description  |
| ------ | --------- | ------------ |
| 0x00   | `AddAtom` | Add new atom |

### Detail of instructions

#### `AddAtom`

| Index | Size | Description |
| ----- | ---- | ----------- |
| 0     | 4    | Register ID |
| 1     | 4    | Membrane ID |
| 2     | 4    | Name ID     |
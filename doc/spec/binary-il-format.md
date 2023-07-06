# IL binary format

The binary format is a compact representation of the IL.
It is designed to be easy to parse and generate.
It is not designed to be human readable.

## Format

### Header

| Offset | Size | Description                                                 |
| ------ | ---- | ----------------------------------------------------------- |
| 0      | 4    | Magic number, always `0x4C4D4E42`, which is `LMNB` in ASCII |
| 4      | 1    | Version number, currently `0x00`                            |
| 5      | 1    | Flags, see below                                            |
| 6      | 2    | Reserved, not used                                          |
| 8      | 4    | Offset of string table                                      |
| 12     | 4    | Size of string table                                        |
| 16     | 4    | Offset of predefinition section                             |
| 20     | 4    | Size of predefinition section                               |
| 24     | 4    | Offset of rule section                                      |
| 28     | 4    | Size of rule section                                        |
| ...    | ...  | string table                                                |
| ...    | ...  | predefinition section                                       |
| ...    | ...  | rule section                                                |


#### Flags

| Bit | Description                      |
| --- | -------------------------------- |
| 0-6 | Reserved                         |
| 7   | 1 if has hyperlinks, 0 otherwise |

### String table

The string table is a list of null-terminated strings.
All items are stored contiguously, with no padding between them.
Padding at the end of the table is allowed.
An item is started with a 1-byte length, followed by the string itself.

### Predefinition section

Predefinition section are stored contiguously, with no padding between them.

### Rule section

Rule section starts with a header, followed by a list of rules.

#### Header

| Offset | Size | Description     |
| ------ | ---- | --------------- |
| 0      | 4    | Number of rules |
| $4n$   | 4    | Offset of rule  |

#### Rule

Rule starts with a header, followed by head, guard and body.

##### Header

Header for rule are used for diagnostics.

| Offset | Size | Description     |
| ------ | ---- | --------------- |
| 0      | 4    | Offset of guard |
| 4      | 4    | Offset of body  |
| 8      | 4    | Membrane ID     |
| 12     | 4    | Priority        |
| 16     | 4    | Name ID         |

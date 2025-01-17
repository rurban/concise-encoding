Concise Binary Encoding and Concise Text Encoding
=================================================

General purpose, compact representations of semi-structured hierarchical data, in binary and text formats.

Alternative To:

* JSON
* XML
* BSON
* CBOR
* MessagePack
* Protobuf



Goals
-----

  * General purpose encoding for a large number of applications
  * Supports the most common data types
  * Supports hierarchical data structuring
  * Minimal complexity
  * Type compatibility between [CBE](cbe-specification.md) and [CTE](cte-specification.md)

#### Goals: Concise Binary Encoding (CBE):

  * Binary format to minimize parsing costs
  * Little endian byte ordering to allow the most common systems to read directly off the wire
  * Balanced space and computation efficiency

#### Goals: Concise Text Encoding (CTE):

  * Human readable format



Supported Types
---------------


### Scalar Types

| Type    | Description                                            |
| ------- | ------------------------------------------------------ |
| Boolean | True or false                                          |
| Integer | Signed two's complement, from 8 to 128 bits            |
| Float   | IEEE 754 floating point, from 32 to 128 bits           |
| Decimal | IEEE 754 densely packed decimal, from 32 to 128 bits   |
| Time    | Date & time, to the microsecond or nanosecond          |


### Array Types

| Type    | Description                                            |
| ------- | ------------------------------------------------------ |
| Bytes   | Array of binary data                                   |
| String  | Array of UTF-8 encoded bytes                           |
| URI     | Universal Resource Identifier                          |
| Comment | A UTF-8 encoded comment string                         |


### Container Types

Containers can hold any combination of types, including other containers.

| Type    | Description                                            |
| ------- | ------------------------------------------------------ |
| List    | A list may containin any types, even mixed             |
| Map     | Scalar or array types for keys, any types for values   |


### Other Types

| Type    | Description                                            |
| ------- | ------------------------------------------------------ |
| Nil     | Denotes the absence of data                            |
| Padding | Used to align data in a CPU friendly manner (CBE Only) |



Specifications
--------------

 * [Concise Binary Encoding](cbe-specification.md)
 * [Concise Text Encoding](cte-specification.md)



Implementations
---------------

* [C implementation](reference-implementation)
* [Go implementation](https://github.com/kstenerud/go-cbe)



License
-------

Specifications released under Creative Commons Attribution 4.0 International Public License.
Reference implementation released under MIT License.

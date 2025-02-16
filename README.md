# create-sriov-vfs

## Features

- Creates specified number of SR-IOV VFs on a specified NIC.

## Build

To build the program, use:
```sh
make
```

## Usage

```sh
create-sriov-vfs <class> <device> <number-of-vfs>
```

## Example
```sh
sudo ./create-sriov-vfs net eth0 4
```

This command will create 4 VFs on the `eth0` network interface.

## Requirements

- A NIC that supports SR-IOV.
- Root privileges to create VFs.

## License

This project is licensed under the MIT License.

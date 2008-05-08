#!/bin/bash

target=$1

if [ -z "$target" ]; then
    echo >&2 "No target device specified."
    exit 1
fi

if [ ! -b "$target" ]; then
    echo >&2 "$target does not appear to be a block device."
    exit 1
fi
    

function getDeviceFromMount ()
{
    # df "$1" | awk '/\// { print $1 }'
    hal-find-by-property --key volume.mount_point --string "$1"
}

function getDeviceFromDevFile ()
{
    hal-find-by-property --key block.device --string "$1"
}

function getDevFile ()
{
    hal-get-property --udi "$1" --key block.device
}

function getMount ()
{
    hal-get-property --udi "$1" --key volume.mount_point
}

function getStorageDevice ()
{
    hal-get-property --udi "$1" --key block.storage_device
}

function getDeviceMB ()
{
    echo $(( $(hal-get-property --udi "$1" --key volume.size) / (1024 * 1024) ))
}

function partitionDevice ()
{
    /sbin/sfdisk "$1" -uM <<EOF
,$2,L
,,L
EOF
}

# /read-only stuff
roudi=$(getDeviceFromMount /read-only)
if [ -z "$roudi" ] ; then
    echo >&2 "Could not get Hal name for /read-only"
    exit 1
fi

rodev=$(getDevFile "$roudi")
if [ -z "$rodev" ] ; then
    echo >&2 "Could not get device for /read-only"
    exit 1
fi

rodrive=$(getStorageDevice "$roudi")
if [ -z "$roudi" ] ; then
    echo >&2 "Could not get drive for /read-only"
    exit 1
fi


# /read-write stuff
rwudi=$(getDeviceFromMount /read-write)
if [ -z "$rwudi" ] ; then
    echo >&2 "Could not get Hal name for /read-write"
    exit 1
fi

rwdev=$(getDevFile "$rwudi")
if [ -z "$rwdev" ] ; then
    echo >&2 "Could not get device for /read-write"
    exit 1
fi

rwdrive=$(getStorageDevice "$rwudi")
if [ -z "$rwdrive" ] ; then
    echo >&2 "Could not get drive for /read-write"
    exit 1
fi



if [ "$rodrive" != "$rwdrive" ] ; then
    echo >&2 "The read-only and read-write partitions seem to be on different devices."
    exit 1
fi



if ! partitionDevice "$target" $(getDeviceMB "$roudi") ; then
    echo >&2 "Partitioning target failed."
    exit 1
fi

if ! dd if="$rodev" of="$target"1 bs=1M ; then
    echo >&2 "Copying read-only partition failed."
    exit 1
fi

# gnome-volume-manager will sometimes mount this against our will
umount "$target"2
if ! /sbin/mkfs -t ext2 "$target"2 ; then
    echo >&2 "Could not create read-write partition."
    exit 1
fi

mnt=$(mktemp -d /mnt/file-shrimp.XXXXXX)
if [ $? != 0 ]; then
    echo >&2 "Could not create temporary directory."
    exit 1
fi

if ! mount "$target"2 "$mnt" ; then
    rmdir "$mnt"
    echo >&2 "Could not mount read-write partition."
fi

if ! cp -avx /read-write/* "$mnt/" ; then
    umount "$mnt"
    rmdir "$mnt"
    echo >&2 "Could not copy read-write partition."
fi

umount "$mnt"
rmdir "$mnt"

exit 0

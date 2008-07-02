#!/bin/bash

function scriptExit ()
{
    killall -CONT gnome-volume-manager
    exit $1
}

function errorDialog ()
{
    echo "# Error: $1"
    echo "100"
    echo >&2 "${0##*/}: $1"
    zenity --error --title "Thin Client Imager" --text "$1"
}

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
    echo $(( $(hal-get-property --udi "$1" --key volume.size) / (1024 * 1024) + 1 ))
}

function partitionDevice ()
{
    # we need the start pos for the 2nd drive because we are leaving
    # space for grub at the beginning of the device, or something.  if
    # we don't specify the start, sfdisk will start at 0 and make us a
    # 1M device.  we add 2 to start after the first device (which
    # started at 1).
    /sbin/sfdisk "$1" -uM >&2 <<EOF
1,$2,L
$(($2 + 2)),,L
EOF
}

target=$1

(
echo "# Initializing..."
if [ -z "$target" ]; then
    errorDialog "No target device specified."
    scriptExit 1
fi

if [ ! -b "$target" ]; then
    errorDialog "$target does not appear to be a block device."
    scriptExit 1
fi

killall -STOP gnome-volume-manager

# /read-only stuff
roudi=$(getDeviceFromMount /read-only)
if [ -z "$roudi" ] ; then
    errorDialog "Could not get Hal name for /read-only"
    scriptExit 1
fi

rodev=$(getDevFile "$roudi")
if [ -z "$rodev" ] ; then
    errorDialog "Could not get device for /read-only"
    scriptExit 1
fi

rodrive=$(getStorageDevice "$roudi")
if [ -z "$roudi" ] ; then
    errorDialog "Could not get drive for /read-only"
    scriptExit 1
fi


# /read-write stuff
rwudi=$(getDeviceFromMount /read-write)
if [ -z "$rwudi" ] ; then
    errorDialog "Could not get Hal name for /read-write"
    scriptExit 1
fi

rwdev=$(getDevFile "$rwudi")
if [ -z "$rwdev" ] ; then
    errorDialog "Could not get device for /read-write"
    scriptExit 1
fi

rwdrive=$(getStorageDevice "$rwudi")
if [ -z "$rwdrive" ] ; then
    errorDialog "Could not get drive for /read-write"
    scriptExit 1
fi



if [ "$rodrive" != "$rwdrive" ] ; then
    errorDialog "The read-only and read-write partitions seem to be on different devices."
    scriptExit 1
fi


targetudi=$(getDeviceFromDevFile "$target")
if [ -z "$targetudi" ] ; then
    errorDialog "Could not get Hal name for target"
    scriptExit 1
fi

for vol in $(hal-find-by-property --key block.storage_device --string "$targetudi") ; do
    if [ "$vol" != "$targetudi" ] ; then
	tmpmnt=$(getMount "$vol")
	if [ "$tmpmnt" ] ; then
	    if ! umount "$tmpmnt" ; then
		errorDialog "Could not unmount $tmpmnt"
		scriptExit 1
	    fi
	fi
    fi
done

echo "# Partitioning target..."
if ! partitionDevice "$target" $(getDeviceMB "$roudi") >/dev/null ; then
    errorDialog "Partitioning target failed."
    scriptExit 1
fi
echo "3"

echo "# Copying system data..."
if ! dd if="$rodev" of="$target"1 bs=1M >/dev/null ; then
    errorDialog "Copying read-only partition failed."
    scriptExit 1
fi
echo "57"

echo "# Formatting configuration partition..."
# gnome-volume-manager will sometimes mount this against our will
umount "$target"2
if ! /sbin/mkfs -t ext2 "$target"2 >/dev/null ; then
    errorDialog "Could not create read-write partition."
    scriptExit 1
fi

mnt=$(mktemp -d /mnt/file-shrimp.XXXXXX)
if [ $? != 0 ]; then
    errorDialog "Could not create temporary directory."
    scriptExit 1
fi

if ! mount "$target"2 "$mnt" ; then
    rmdir "$mnt"
    errorDialog "Could not mount read-write partition."
    scriptExit 1
fi
echo "60"

echo "# Copying configuration data..."
if ! cp -ax /read-write/* "$mnt/" ; then
    umount "$mnt"
    rmdir "$mnt"
    errorDialog "Could not copy read-write partition."
    scriptExit 1
fi
echo "96"

echo "# Saving home directory..."
rm -rf "$mnt/etc/userlevel/home"
if ! cp -ax "$HOME" "$mnt/etc/userlevel/home" ; then
    umount "$mnt"
    rmdir "$mnt"
    errorDialog "Could not save home directory."
    scriptExit 1
fi
echo "97"

echo "# Installing boot loader..."
echo "device (hd0) $target" > /tmp/grub.conf
echo "root (hd0,1) $target" >> /tmp/grub.conf
echo "setup (hd0)" >> /tmp/grub.conf
echo "quit" >> /tmp/grub.conf
if ! /usr/sbin/grub --batch < /tmp/grub.conf >/tmp/grub.log; then
    umount "$mnt"
    rmdir "$mnt"
    errorDialog "Could not install GRUB on $target"
    scriptExit 1
fi
echo "98"

echo "# Cleaning up..."
umount "$mnt"
rmdir "$mnt"
echo "99"

echo "# Finished installing image to $target..."
echo "100"
) | 
zenity --progress \
    --title="Thin Client Image Tool" \
    --auto-close \
    --text="" \
    --percentage=0

scriptExit 0

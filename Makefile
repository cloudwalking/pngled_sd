sd:
	cp *.dat /Volumes/SD/
	ls -la /Volumes/SD/

unmount:
	diskutil unmountDisk /dev/disk1

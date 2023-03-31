#include <common.h>
#include <part.h>
#include <config.h>
#include <command.h>
#include <image.h>
#include <linux/ctype.h>
#include <asm/byteorder.h>
#include <ext4fs.h>
#include <linux/stat.h>
#include <malloc.h>
#include <fs.h>

#define VAR_MAX_LEN 64

int do_readvar(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	loff_t len_read;
	int ret;
	char buffer[VAR_MAX_LEN+1];
	int i;

	if (argc != 5)
		return CMD_RET_USAGE;

	if (fs_set_blk_dev(argv[1], argv[2], FS_TYPE_ANY))
	{
		printf("Can't use %s %s as filesystem\n", argv[1], argv[2]);
		return 1;
	}

	ret = fs_read(argv[3], (ulong)buffer, 0, sizeof(buffer)-1, &len_read);

	if (ret < 0)
	{
		printf("Failed to read %s. Error code %d\n", argv[3], ret);
		return 1;
	}

	for(i=0; i<len_read; i++)
		if(buffer[i] < ' ')
			break;
	buffer[i]=0;

	env_set(argv[4], buffer);

	return 0;
}

U_BOOT_CMD(readvar, 5, 0, do_readvar,
	   "read file from filesystem and assign content to environment variable\n",
	   "<interface> <dev[:part]> <file_name> <variable_name>\n"
);

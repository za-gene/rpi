/* dump the partition table
 */

#include "fat32.h"

int main()
{

	fat32_init();
	fat32_type_partition_table();
	fat32_deinit();
	return 0;
}

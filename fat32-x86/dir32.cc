/* dump the partition table
 */

#include "fat32.h"

int main()
{

	fat32_init();
	fat32_list_root();
	fat32_deinit();
	return 0;
}

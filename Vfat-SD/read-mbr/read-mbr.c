

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE * in = fopen("../mnt/tf.img", "rb");
    unsigned int i, start_sector, length_sectors;
    
    fseek(in, 0x1BE, SEEK_SET); // go to partition table start
    
    for(i = 0; i < 4; i++) 
    { // read all four entries
        printf("Partition entry %d: First byte %02X \r\n", i, fgetc(in));
        printf("  Partition start in CHS: %02X:%02X:%02X \r\n", fgetc(in), fgetc(in), fgetc(in));
        printf("  Partition type %02X \r\n", fgetc(in));
        printf("  Partition end in CHS: %02X:%02X:%02X \r\n", fgetc(in), fgetc(in), fgetc(in));
        
        fread(&start_sector, 4, 1, in);
        fread(&length_sectors, 4, 1, in);
        printf("  Relative LBA address %08X, %d sectors long \r\nn", start_sector, length_sectors);
    }
    
    fclose(in);
    return 0;
}

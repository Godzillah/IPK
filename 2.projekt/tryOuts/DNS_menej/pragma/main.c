#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct R_DATA
{
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;
};
#pragma pack(pop)

int main () {

	struct R_DATA *r_data;
	int size;
	unsigned short variable;

	size = sizeof(r_data);
	printf("Size of structure is :%d.\n" , size);
	printf("Size of unsigned short is: %d.\n",sizeof(variable));
	return 0;


}
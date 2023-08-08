#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void check_elf(unsigned char *e_ident);
void print_field(const char *field_name, unsigned int value, const char *format);
void print_entry(unsigned long int e_entry, unsigned char *e_ident);
void close_elf(int elf);

void check_elf(unsigned char *e_ident)
{
	int index;

	for (index = 0; index < 4; index++)
	{
		if (e_ident[index] != 127 &&
		    e_ident[index] != 'E' &&
		    e_ident[index] != 'L' &&
		    e_ident[index] != 'F')
		{
			dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
			exit(98);
		}
	}
}

void print_field(const char *field_name, unsigned int value, const char *format)
{
	printf("  %-32s", field_name);
	printf(format, value);
	printf("\n");
}

void print_entry(unsigned long int e_entry, unsigned char *e_ident)
{
	printf("  Entry point address: ");

	if (e_ident[EI_DATA] == ELFDATA2MSB)
	{
		e_entry = ((e_entry << 8) & 0xFF00FF00) |
			  ((e_entry >> 8) & 0xFF00FF);
		e_entry = (e_entry << 16) | (e_entry >> 16);
	}

	if (e_ident[EI_CLASS] == ELFCLASS32)
		printf("%#x\n", (unsigned int)e_entry);
	else
		printf("%#lx\n", e_entry);
}

void close_elf(int elf)
{
	if (close(elf) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", elf);
		exit(98);
	}
}

int main(int argc, char *argv[])
{
	Elf64_Ehdr *header;
	int o, r;

	o = open(argv[1], O_RDONLY);
	if (o == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}

	header = malloc(sizeof(Elf64_Ehdr));
	if (header == NULL)
	{
		close_elf(o);
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}

	r = read(o, header, sizeof(Elf64_Ehdr));
	if (r == -1)
	{
		free(header);
		close_elf(o);
		dprintf(STDERR_FILENO, "Error: `%s`: No such file\n", argv[1]);
		exit(98);
	}

	check_elf(header->e_ident);
	printf("ELF Header:\n");

	print_field("Magic:", header->e_ident, "%02x %02x %02x %02x");
	print_field("Class:", header->e_ident[EI_CLASS], "%s", header->e_ident[EI_CLASS] == ELFCLASS32 ? "ELF32" : "ELF64");
	print_field("Data:", header->e_ident[EI_DATA], "%s");
	print_field("Version:", header->e_ident[EI_VERSION], "%d");
	print_field("OS/ABI:", header->e_ident[EI_OSABI], "%s");

	print_field("ABI Version:", header->e_ident[EI_ABIVERSION], "%d");
	print_field("Type:", header->e_type, "%s");
	print_entry(header->e_entry, header->e_ident);

	free(header);
	close_elf(o);
	return (0);
}

#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main(int argc, char *argv[]) {
	FILE *yves;
	FILE *html;
	char *html_filename;
	unsigned char yves_data[2];

	if (argc != 2)
	{
		printf("provide a yves filename as the parameter\n");
		return 0;
	}

	yves = fopen(argv[1], "rb");
	if (yves == NULL)
	{
		printf("file %s not found\n", argv[1]);
		return 1;
	}

	html_filename = (char *)malloc(strlen(argv[1]) + 6);
	strcat(html_filename, argv[1]);
	strcat(html_filename, ".html");
	printf("> Creating %s\n", html_filename);
	html = fopen(html_filename, "wb");

	while (fread(yves_data, 1, 2, yves) > 0)
	{
		yves_data[0] = ((yves_data[0] >> 5) & 0xff) | ((yves_data[0] << 3) & 0xff);
		yves_data[1] = ((yves_data[1] >> 5) & 0xff) | ((yves_data[1] << 3) & 0xff);
		yves_data[0] = (yves_data[0] ^ yves_data[1]) & 0xff;
		yves_data[1] = (yves_data[0] ^ yves_data[1]) & 0xff;
		yves_data[0] = (yves_data[0] ^ yves_data[1]) & 0xff;
		if (yves_data[0] != 0)
		{
			fputc(yves_data[0], html);
		}
		fputc(yves_data[1], html);
		yves_data[1] = 0;
	}

	free(html_filename);
	fclose(yves);
	fclose(html);
	printf("done\n");
	return 0;
}

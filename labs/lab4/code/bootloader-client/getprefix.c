
        int i;
        printf("const char prefix[] = {\n");
        for(i = 0; i < 16; i++)
                printf("\t0x%x,\n", (unsigned char)buf[i]);
        printf("0\n};\n");
        printf("-----------------------------------------------------------\n");



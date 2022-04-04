/*
 * The Tree Command
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 3
 */

#include <infos.h>


const char *cat(const char *lhs, const char *rhs)
{

	const char *a = lhs;
	const char *b = rhs;

	// int new_size = strlen(a) + strlen(b);

  printf("strlen a %d \n", strlen(a));

	char buffer[strlen(a) + strlen(b)];

	int i = 0;
	while (*a && *a != ' ') {
  	printf("%d", i);
		buffer[i++] = *a++;
	}
	buffer[i] = *"/";
  i++;

	if (*a) a++;


	printf("current state %s \n", buffer);


	while (*b && *b != ' ') {
		buffer[i++] = *b++;
	}
	buffer[i] = 0;

	if (*b) b++;

	printf("final state %s \n", buffer);

  const char* ret = buffer;
  return ret;

}


void print(const char* name, int level) {

	int lvl = level;

	while (lvl != 0) {
		printf("|   ");
		lvl--;
	}


	printf("|--- ");


	// prints the name of the dir or file
	printf("%s\n", name);
}



int recurse_dir(const char* dir_name, int level) {

//   const char *test = cat("hello", "world");
//   printf("in rd %s \n", test);
  // return 0;
	HDIR dir = opendir(dir_name, 0);

  // if (is_error(dir)) {
  //     printf("big time uh-oh");
  //     return 1;
  // }


	struct dirent de;
  bool is_dir = false;
  while (readdir(dir, &de)) {
        // printf("%s (%u bytes) \n", de.name, de.size);

        // const char* new_dir = cat(dir_name, de.name);













				const char *a = dir_name;
				const char *b = de.name;


				char buffer[strlen(a) + strlen(b)];
				int i = 0;
				while (*a && *a != ' ') {
		    	buffer[i++] = *a++;
	    	}
	    	buffer[i] = *"/";
        i++;

	    	if (*a) a++;




	    	while (*b && *b != ' ') {
		    	buffer[i++] = *b++;
	    	}
	    	buffer[i] = 0;

	    	if (*b) b++;


      	const char* new_dir = buffer;





















				print(de.name, level);

        HDIR dir2 = opendir(new_dir, 0);
        // printf("%s\n", de.name);
        if (!is_error(dir2)){
            // printf("d\n");
            is_dir = true;
        } else {
            // printf("nd\n");
        }

        if (is_dir) {
            recurse_dir(new_dir, level+1);
        }
        // struct dirent de2;
        // // printf("the new name is %s\n", new_name);
        // while (readdir(dir2, &de2)) {

        //     printf("=-=-= %s (%u bytes) \n", de2.name, de2.size);
        // }
        is_dir = false;
    }

    return 0;
}







int main(const char *cmdline)
{
    // TODO: Implement me!
    const char* input = cmdline;


    // default is no input directory, which just prints the default location (/usr)
    if (!input || strlen(input) == 0) {
        printf("case: default \n");
        // printf("directory: /usr \n");
        input = "/usr";
    } else {
        // printf("non-null, we have - %s\n", input);

				bool regex = false;
				int dir_end_pos = 0;


				while (*input) {

					dir_end_pos++;

					if (*input++ == *" "){


						if (*input++ == *"-" && *input++ == *"P"){
							//break out as regex is true
							dir_end_pos+2;

							// parse_dir[dir_end_pos] = 0;

							input++;
							// printf("helloooo %s\n", parse_dir);
							regex=true;
							break;
						}
					}

	    	}


				char buffer[dir_end_pos];
				const char* parse_dir = cmdline;

				for (unsigned int i=0; i<dir_end_pos; i++) {
					buffer[i] = *parse_dir++;
				}
				buffer[dir_end_pos] = 0;

				parse_dir = buffer;

				printf("this is the moment %s\n", parse_dir);

				// so we now have the regex pattern
				printf("this is the regex pattern %s\n", input);
				// what we need is the location (if specified)

				// if (regex) {
				// 	// input should be at regex begin
				// 	// printf("regex length? (%d)", strlen(input));
				// 	char buffer[strlen(input)];
				//
				// 	// loop over the remainder of the input, the regex
				// 	int i=0;
				// 	while(*input && *input != ' ') {
				// 		buffer[i++] == *input++;
				// 	}
				// 	buffer[i] = 0;
				//
				// 	if(*input) input++;
				//
				// 	const char* regex = buffer;
				//
				// 	printf("this is crazy %s", regex);
				// }



    }

    // now to open the input directory

    // recurse_dir(input, NULL, 0);


    // printf("%s\n", cat("hello", "world"));
    // const char *test = cat("hello", "world");
    // printf("in main %s \n", test);
    recurse_dir(input, 0);






    return 0;
}

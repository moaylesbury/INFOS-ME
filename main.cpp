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




int recurse_dir(const char* dir_name) {

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





















        
        printf("%s\n", new_dir);
        HDIR dir2 = opendir(new_dir, 0);
        // printf("%s\n", de.name);
        if (!is_error(dir2)){
            // printf("d\n");
            is_dir = true;
        } else {
            // printf("nd\n");
        }

        if (is_dir) {
            recurse_dir(new_dir);
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
        printf("non-null, we have - %s", input);
    }

    // now to open the input directory

    // recurse_dir(input, NULL, 0);


    // printf("%s\n", cat("hello", "world"));
    // const char *test = cat("hello", "world");
    // printf("in main %s \n", test);
    recurse_dir(input);






    return 0;
}


// pasted from ls
// {

// 	HDIR dir = opendir(path, 0);
// 	if (is_error(dir)) {
// 		printf("Unable to open directory '%s' for reading.\n", path);
// 		return 1;
// 	}

// 	printf("Directory Listing of '%s':\n", path);

// 	struct dirent de;
// 	while (readdir(dir, &de)) {
// 		printf("  %s (%u bytes)\n", de.name, de.size);
// 	}
// 	closedir(dir);

// 	return 0;
// }

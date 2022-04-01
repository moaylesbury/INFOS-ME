/*
 * The Tree Command
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 3
 */

#include <infos.h>

// HDIR get_concat_dir(const char* lhs, const char* rhs){
//     // concatenates two strings
    
//     int lhs_size = strlen(lhs);
//     int rhs_size = strlen(rhs);

//     // printf("this word %s has size %d \n", lhs, lhs_size);
//     // printf("this word %s has size %d \n", rhs, rhs_size);

//     int new_size = lhs_size + rhs_size;

//     char concat[new_size+1];
//     concat[new_size+1] = 0;
//     // char* concat = new char[new_size];
//     // _data = new char[_size + 1];
    

//     // printf("the new size is %d \n", new_size);

//     // need to add the slash at the relevant location
    
   

//     for (unsigned int i=0; i < lhs_size; i++) {
//         // printf("%d\n", i);
//         concat[i] = lhs[i];
//     }
//     concat[lhs_size] = *"/";

//     printf("\n");

//     for (unsigned int i=0; i < rhs_size; i++) { 
//         // printf("%d\n", lhs_size+i);
//         concat[lhs_size+i+1] = rhs[i];
//     }


//     // printf("\n");
//     // const char* i = concat;
//     // printf("concatenation: %s\n", concat);
//     return opendir(concat, 0);

// }



// int recurse_dir(HDIR dir) {

    

    
//     struct dirent de;
//     bool is_dir = false;
//     while (readdir(dir, &de)) {
//         // printf("%s (%u bytes) \n", de.name, de.size);
        
//         HDIR dir2 = get_concat_dir(dir_name, de.name);
//         if (!is_error(dir2)){
//             printf("is a directory");
//             is_dir = true;
//         } else {
//             printf("not a directory");
//         }

//         if (is_dir) {
//             recurse(dir2);
//         }
//         // struct dirent de2;
//         // // printf("the new name is %s\n", new_name);
//         // while (readdir(dir2, &de2)) {
        
//         //     printf("=-=-= %s (%u bytes) \n", de2.name, de2.size);
//         // }
//         is_dir = false;
//     }

//     return 0;
// }

// void print_dir(int recursion_level) {
//     printf("pass");
// }

// const char* concatenate(const char* lhs, const char* rhs) {
//     // concatenates two strings
//     // takes lhs, rhs, returns lhs/rhs
    
//     int lhs_size = strlen(lhs);
//     int rhs_size = strlen(rhs);

//     // printf("this word %s has size %d \n", lhs, lhs_size);
//     // printf("this word %s has size %d \n", rhs, rhs_size);

//     int new_size = lhs_size + rhs_size;

//     char *buffer[new_size+1];


//     for (unsigned int i=0; i < lhs_size; i++) {
//         // printf("%d\n", i);
//         buffer[i] = lhs[i];
//     }
//     concat[lhs_size] = "/";

//     printf("\n");

//     for (unsigned int i=0; i < rhs_size; i++) { 
//         // printf("%d\n", lhs_size+i);
//         buffer[lhs_size+i+1] = rhs[i];
//     }


    

//     return buffer;
// } 

const char *cat(const char *lhs, const char *rhs)
{

	const char *a = lhs;
	const char *b = rhs;

	int new_size = strlen(a) + strlen(b);

    printf("strlen a %d \n", strlen(a));

	char buffer[new_size];

	int i = 0;
	while (*a && *a != ' ') {
        printf("%d \n", i);
		buffer[i++] = *a++;
	}
	buffer[i] = 0;

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

    // HDIR dir = opendir(dir_name, 0);

    // if (is_error(dir)) {
    //     printf("big time uh-oh");
    //     return 1;
    // }

    const char *test = cat("hello", "world");
    printf("PLEASE GOD %s \n", test);
    // recurse_dir(dir);
    





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

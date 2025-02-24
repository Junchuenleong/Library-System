#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_COPY 1000
#define MAX_LEN 100
#define MAX_BOOKS 10
#define MAX_RESULTS 100
#define MAX_DAYS 7

//Created a struct to borrow books
typedef struct bookInfo {
    char ISBN[MAX_LEN];
    time_t borrowTime;
} bookInfo;

typedef struct student {
    char studentID[MAX_LEN];
    char stdName[MAX_LEN];
    int numOfBooks;
    bookInfo time_since_borrowed[MAX_BOOKS];
    int accessRight; // 1 for student 2 for librarians
} student;

typedef struct book {
    char ISBN[MAX_LEN];
    char Title[MAX_LEN];
    char author[MAX_LEN];
    char publication_year[MAX_LEN];
    int numCopies;
    int numBorrowed;
} book;

//Struct with array of struct books
typedef struct SearchResult{
    book books[MAX_RESULTS];
    int count;
} SearchResult;

//Struct to store transaction
typedef struct Transaction {
    char type[MAX_LEN]; // "borrow" or "return"
    char ISBN[MAX_LEN];
    char studentID[MAX_LEN];
    time_t date;
    int fine;
} Transaction;

//Function prototype
void modifyBookCopies(char *ISBN);
void printSeparater(int amount);
int daysSinceBorrowed(time_t borrowTime);
int isAlphabetic(const char *str);
int isCapital(const char *str);
int getValidInteger();
int isPhrasePresent(const char *name, const char *phrase);
book findBookByISBN(char *ISBN);
void printBookStruct(book foundBook);
void addStudentToFile() ;
student findStudentByID(const char *studentID);
void printStudentDetails(student *std);
void swapBooks(book *a, book *b);
void sortByTitle(book *books, int count);
void displayAllBooks();
SearchResult searchBookByName(const char *phrase);
void printSimilarBooks(SearchResult result);
void addBookToFile();
void borrowBook(student *std, char *ISBN);
int daysSinceBorrowed(time_t borrowTime);
void returnBook(student *std, char *ISBN);
void appendTransactionDetails(char *transactionType, char *ISBN, char *studentID, int fine);
void generateMonthlyReport(int month, int year);
void displayStudentTransactions(const char *studentID, int month, int year);
void clearInputBuffer();
void displayBookTransactions(const char *ISBN, int month, int year);
void getISBN();
void updateStudentFile(student *updatedStudent);
void updateBookFile(const book *updatedBook);

//Function to print -
void printSeparater(int amount){
    for (int i = 0; i < amount; i++){
     printf("-");
    }
    puts("");
}

//Function to clear input buffer
void clearInputBuffer (){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

//Function to receive ISBN from stdin
void getISBN(char *ISBN) {
    printf("Enter ISBN:\n");
    fgets(ISBN, MAX_LEN, stdin);
    ISBN[strcspn(ISBN, "\n")] = '\0'; // Remove newline character
}

// Function to check if a string contains only alphabetic characters
int isAlphabetic(const char *str) {
    while (*str) {
        if (!isalpha(*str)) {
            return 0; // Return 0 if any character is not alphabetic
        }
        str++;
    }
    return 1; // Return 1 if all characters are alphabetic
}

// Function to check if the first character of a string is a capital letter
int isCapital(const char *str) {
    return (str[0] >= 'A' && str[0] <= 'Z');
}

//Check for integer input for element
int getValidInteger() {
    int num;
    char d;
        //Scanf condtion takes in an integer and a character (d)
        //If the character is not '\n' -> input is invalid
    while (scanf("%d%c", &num, &d) != 2 || d != '\n') {
        printf("Invalid input. Please enter an integer: ");
        clearInputBuffer(); // Clear input buffer
    }
    return num;
}

//Check if specific phrase is present
int isPhrasePresent(const char *name, const char *phrase) {
    const char *ptr = name;
    int phraseLen = strlen(phrase);
    
    while ((ptr = strstr(ptr, phrase)) != NULL) {
        if ((ptr == name || !isalpha(*(ptr - 1))) &&
            (!isalpha(ptr[phraseLen]) || ptr[phraseLen] == '\0')) {
            return 1; // Return 1 if the phrase is found as a whole word
        }
        ptr += phraseLen; // Move the pointer forward
    }
    return 0; // Return 0 if the phrase is not found
}

// Function to find a book by ISBN and return it
// Return book struct with ISBN "" if error
book findBookByISBN(char *ISBN) {

    FILE *bookPtr;
    book currentBook, foundBook;

    bookPtr = fopen("book.txt", "r");

    if (bookPtr == NULL) {
        printf("Error opening file.\n");
        strcpy(foundBook.ISBN ,"");
        fclose(bookPtr);
        return foundBook;
    }
    char line[MAX_COPY];
    
    int found = 0;

    while (fgets(line, sizeof(line), bookPtr)) {
        char *token = strtok(line, "|");
        strcpy(currentBook.ISBN, token);

        // If the ISBN matches, retrieve the book details
        if (strcmp(currentBook.ISBN, ISBN) == 0) {
            token = strtok(NULL, "|");
            strcpy(currentBook.Title, token);
            token = strtok(NULL, "|");
            strcpy(currentBook.author, token);
            token = strtok(NULL, "|");
            strcpy(currentBook.publication_year, token);
            token = strtok(NULL, "|");
            currentBook.numCopies = atoi(token);
            token = strtok(NULL, "|");
            currentBook.numBorrowed = atoi(token);

            foundBook = currentBook;
            found = 1; // Set the flag for book found
            fclose(bookPtr);
            break;
        }
    }

    if (!found) {
        strcpy(foundBook.ISBN, ""); // Empty string denotes book not found
        fclose(bookPtr);
    }
    fclose(bookPtr);
    return foundBook;
}

//Function to print out book details
void printBookStruct(book foundBook) {
    if (strcmp(foundBook.ISBN, "") != 0) {
        // Book found, print details
        printf("Book found!\n");
        printf("ISBN: %s\n", foundBook.ISBN);
        printf("Book Name: %s\n", foundBook.Title);
        printf("Author: %s\n", foundBook.author);
        printf("Publication year: %s\n", foundBook.publication_year);
        int numLeft = foundBook.numCopies - foundBook.numBorrowed;
        printf("Number of Copies Available: %d\n", numLeft);
        printf("Total Copies: %d\n", foundBook.numCopies);
    } else {
        printf("Book not found.\n");
    }
}

//Function to add a student to file
void addStudentToFile() {
    FILE *StudentPtr = fopen("student.txt", "a+");
    if (StudentPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    student newStudent;
    printf("Enter student ID: ");
    fgets(newStudent.studentID, sizeof(newStudent.studentID), stdin);
    newStudent.studentID[strcspn(newStudent.studentID, "\n")] = '\0';

    // Check if student ID already exists
    char line[MAX_COPY];
    while (fgets(line, sizeof(line), StudentPtr) != NULL) {
        char *token = strtok(line, "|");
        if (strcmp(token, newStudent.studentID) == 0) {
            printf("Student ID already exists.\n");
            fclose(StudentPtr);
            return;
        }
    }
    
    fclose(StudentPtr);

    // If the student ID doesn't exist, continue adding the student to the file
    StudentPtr = fopen("student.txt", "a");
    if (StudentPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter the name of student: ");
    do {
        fgets(newStudent.stdName, sizeof(newStudent.stdName), stdin);
        size_t len = strlen(newStudent.stdName);
        if (len > 0 && newStudent.stdName[len - 1] == '\n') {
            newStudent.stdName[len - 1] = '\0'; // Remove newline character
        }
        if (!isAlphabetic(newStudent.stdName) || !isCapital(newStudent.stdName)) {
            printf("Invalid input. Please enter alphabetic characters with the first letter capitalized: ");
        }
    } while (!isAlphabetic(newStudent.stdName) || !isCapital(newStudent.stdName) || strlen(newStudent.stdName) == 0);

    printf("Enter access right (1 for student, 2 for librarians): ");
    do {
        scanf("%d", &newStudent.accessRight);
        getchar(); // Consume newline
    } while (newStudent.accessRight != 1 && newStudent.accessRight != 2);

    newStudent.numOfBooks = 0;

    //Initialse all borrowed book to null
    for (int i = 0; i < MAX_BOOKS; i++) {
        strcpy(newStudent.time_since_borrowed[i].ISBN, " ");
        newStudent.time_since_borrowed[i].borrowTime = 0;
    }

    fprintf(StudentPtr, "%s|%s|%d|", newStudent.studentID, newStudent.stdName, newStudent.numOfBooks);

    // Write the array data
    for (int i = 0; i < MAX_BOOKS; ++i) {
        fprintf(StudentPtr, "%s,%d,", newStudent.time_since_borrowed[i].ISBN, newStudent.time_since_borrowed->borrowTime);
    }
   
    fprintf (StudentPtr,"%d|",newStudent.accessRight);
    
    fprintf(StudentPtr, "\n"); // End of record (if needed)
    fclose(StudentPtr);

    printf("Student added successfully to student.txt\n");
}


// Function to find a student by ID and return the student struct
//eg call: student foundStudent = findStudentByID("1234");
student findStudentByID(const char *studentID) {
    FILE *StudentPtr = fopen("student.txt", "r");
    student emptyStudent;
    if (StudentPtr == NULL) {
        printf("Error opening file!\n");
        strcpy(emptyStudent.studentID, "");
        return emptyStudent; // Return an empty student for failure
    }

    char line[MAX_COPY];
    while (fgets(line, sizeof(line), StudentPtr) != NULL) {
        char *token = strtok(line, "|");
        if (strcmp(token, studentID) == 0) {
            student foundStudent;
            strcpy(foundStudent.studentID, token);
            token = strtok(NULL, "|");
            strcpy(foundStudent.stdName, token);
            token = strtok(NULL, "|");
            foundStudent.numOfBooks = atoi(token);

            // Since next MAX_BOOKS tokens represent book information
            for (int i = 0; i < MAX_BOOKS; ++i) {
                // Extract ISBN
                token = strtok(NULL, ",");
                if (token != NULL) {
                    strcpy(foundStudent.time_since_borrowed[i].ISBN, token);
                }
                // Extract borrowTime
                token = strtok(NULL, ",");
                if (token != NULL) {
                    foundStudent.time_since_borrowed[i].borrowTime = atoi(token);
                }
            }
            token = strtok(NULL, "|");
            foundStudent.accessRight = atoi(token);

            fclose(StudentPtr);
            return foundStudent; // Return the found student
        }
    }
    fclose(StudentPtr);
    strcpy(emptyStudent.studentID, "");
    return emptyStudent; // Return an empty student if not found
}

// Function to print the details of a given student
//Passing in struct
void printStudentDetails(student *std) {
    printf("Student ID: %s\n", std->studentID);
    printf("Student Name: %s\n", std->stdName);
    printf("Number of Books Borrowed: %d\n", std->numOfBooks);

    //Check if student borrowed any book, if yes output them
    if (std->numOfBooks != 0){
        printf("Books Borrowed:\n");
        for (int i = 0; i < std->numOfBooks; i++) {
            char borrowDate[20];
            struct tm *tm_info = localtime(&(std->time_since_borrowed[i].borrowTime));
            strftime(borrowDate, sizeof(borrowDate), "%Y-%m-%d", tm_info);

            time_t now = time(NULL);
            double seconds = difftime(now, std->time_since_borrowed[i].borrowTime);
            int daysElapsed = seconds / (60 * 60 * 24);

            printf("  Book %d - ISBN: %s, Borrow Date: %s, Days since borrow: %d\n", 
                   i+1, std->time_since_borrowed[i].ISBN, borrowDate, daysElapsed);
        }
    }
    else {
        printf("Books Borrowed: NULL\n");
    }

    printf("Access Right: %d\n", std->accessRight);
}

// Function to swap two books in the array
void swapBooks(book *a, book *b) {
    book temp = *a;
    *a = *b;
    *b = temp;
}

// Function to perform sorting based on book titles
void sortByTitle(book *books, int count) {
    int i, j;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            // Compare titles and perform swapping
            if (strcmp(books[j].Title, books[j + 1].Title) > 0) {
                swapBooks(&books[j], &books[j + 1]);
            }
        }
    }
}

//Function to print all book in tabular form
void displayAllBooks() {
    FILE *bookPtr = fopen("book.txt", "r");
    if (bookPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    book books[MAX_RESULTS];
    int count = 0;
    char line[MAX_COPY];

    // Read books from the file and store them in an array
    while (fgets(line, sizeof(line), bookPtr)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%d\n", books[count].ISBN, books[count].Title, books[count].author,
               books[count].publication_year, &books[count].numCopies, &books[count].numBorrowed);
        count++;
    }
    fclose(bookPtr);

    if (count == 0) {
        printf("No books found.\n");
        return;
    }

    // Sort the array of books by title
    sortByTitle(books, count);

    // Display the sorted list of books in tabular format
    printf("%-17s | %-40s | %-30s | %-15s | %-10s | %-10s\n",
           "ISBN", "Title", "Author", "Publication Year", "Available", "Total");
    for (int i = 0 ; i < 130 ; i++){
        printf ("-");
    }
    puts ("");
    
    for (int i = 0; i < count; i++) {
        printf("%-15s | %-40s | %-30s | %-15s | %-10d | %-10d\n",
               books[i].ISBN, books[i].Title, books[i].author, books[i].publication_year,
               (books[i].numCopies - books[i].numBorrowed), books[i].numCopies);
    }
}

SearchResult searchBookByName(const char *phrase) {
    FILE *bookPtr = fopen("book.txt", "r");
    SearchResult result = { .count = 0 };

    if (bookPtr == NULL) {
        printf("Error opening file!\n");
        return result;
    }

    char line[MAX_COPY];
    book currentBook;

    //Read each elements separated by tokens
    while (fgets(line, sizeof(line), bookPtr)) {
        char lineCopy[MAX_COPY];
        strcpy(lineCopy, line);

        char *token = strtok(lineCopy, "|");
        strcpy(currentBook.ISBN, token);

        token = strtok(NULL, "|");
        strcpy(currentBook.Title, token);

        token = strtok(NULL, "|");
        strcpy(currentBook.author, token);

        token = strtok(NULL, "|");
        strcpy(currentBook.publication_year, token);

        token = strtok(NULL, "|");
        currentBook.numCopies = atoi(token);

        token = strtok(NULL, "|");
        currentBook.numBorrowed = atoi(token);

        const char *delimiters = "|"; // Define delimiters 

        char titleCopy[MAX_LEN];
        strcpy(titleCopy, currentBook.Title);

        token = strtok(titleCopy, delimiters);
        while (token != NULL) {
            if (strstr(token, phrase) != NULL) {
                if (result.count < MAX_RESULTS) {
                    result.books[result.count++] = currentBook;
                } else {
                    printf("Maximum results reached!\n");
                    fclose(bookPtr);
                    return result;
                }
                break;
            }
            token = strtok(NULL, delimiters);
        }
    }

    fclose(bookPtr);
    return result;
}

// Output all similar book when calling search by title
void printSimilarBooks(SearchResult result){
    if (result.count != 0){
        printf("Matching books:\n");
        for (int i = 0; i < result.count; ++i) {
            printf("Book %d:\n", i + 1);
            printf("ISBN: %s\n", result.books[i].ISBN);
            printf("Title: %s\n", result.books[i].Title);
            printf("Author: %s\n", result.books[i].author);
            printf("Publication Year: %s\n", result.books[i].publication_year);
            printf("Number of copies available: %d\n", (result.books[i].numCopies - result.books[i].numBorrowed));
            printf("Total Copies: %d\n", result.books[1].numCopies);
            
            printf("\n");
        }
    }
    else {
        printf ("Book not found");
    }
}

//Funtion to add book to file
void addBookToFile() {
    FILE *BookPtr = fopen("book.txt", "a+");
    if (BookPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    book newBook;
    int validISBN = 0;

    // Receiving ISBN
    do {
        printf("Enter ISBN of the book: ");
        fgets(newBook.ISBN, sizeof(newBook.ISBN), stdin);
        newBook.ISBN[strcspn(newBook.ISBN, "\n")] = '\0'; // Remove newline character

        // Check if ISBN already exists
        book existingBook = findBookByISBN(newBook.ISBN);
        if (strcmp(existingBook.ISBN, "") != 0) {
            printf("ISBN already exists. Please enter a unique ISBN: ");
        } else {
            validISBN = 1;
        }
    } while (!validISBN);


    //Receiving book title
    printf("Enter title of the book: ");
    do {
        fgets(newBook.Title, sizeof(newBook.Title), stdin);
        newBook.Title[strcspn(newBook.Title, "\n")] = '\0'; // Remove newline character
        if (strlen(newBook.Title) == 0) {
            printf("Title cannot be empty. Please enter a valid Title: ");
        } else if (!isCapital(newBook.Title)) {
            printf("The first letter of the title should be capitalized. Please re-enter: ");
        }
    } while (strlen(newBook.Title) == 0 || !isCapital(newBook.Title));

    //Receiving Author Name
    printf("Enter author of the book: ");
    do {
        fgets(newBook.author, sizeof(newBook.author), stdin);
        newBook.author[strcspn(newBook.author, "\n")] = '\0'; // Remove newline character
        if (strlen(newBook.author) == 0) {
            printf("Author cannot be empty. Please enter a valid Author name: ");
        }
    } while (strlen(newBook.author) == 0);

    //Receiving publication year
    printf("Enter publication year of the book: ");
    do {
        fgets(newBook.publication_year, sizeof(newBook.publication_year), stdin);
        newBook.publication_year[strcspn(newBook.publication_year, "\n")] = '\0'; // Remove newline character
        //Check non zero and span of character containing numbers is not equal to the length of string
        if (strlen(newBook.publication_year) == 0 || strspn(newBook.publication_year, "0123456789") != strlen(newBook.publication_year)) {
            printf("Publication year cannot be empty. Please enter a valid integer year: ");
        }
    } while (strlen(newBook.publication_year) == 0 || strspn(newBook.publication_year, "0123456789") != strlen(newBook.publication_year));
    

//Receiving number of copies
   int validInput = 0;
    do {
        printf("Enter numberOfCopies: ");
        char numCopiesInput[MAX_LEN];
        fgets(numCopiesInput, sizeof(numCopiesInput), stdin);
        numCopiesInput[strcspn(numCopiesInput, "\n")] = '\0'; // Remove newline character

        // Check if the input is not empty and contains only digits
        if (strlen(numCopiesInput) == 0 || strspn(numCopiesInput, "0123456789") != strlen(numCopiesInput)) {
            printf("Number of copies cannot be empty and must be a valid integer: ");
        } else {
            // Convert input to integer
            int numCopies;
            sscanf(numCopiesInput, "%d", &numCopies);

            // Check if the entered number is greater than 0
            if (numCopies > 0) {
                validInput = 1;
                newBook.numCopies = numCopies; // Assign the validated value to newBook.numCopies
            } else {
                printf("Number of copies must be a positive integer: ");
            }
        }
    } while (!validInput);

    //Initialised book borrowed to 0
    newBook.numBorrowed = 0;

    //Print to file
    fprintf(BookPtr, "%s|%s|%s|%s|%d|%d\n",
    newBook.ISBN, newBook.Title, newBook.author, newBook.publication_year, newBook.numCopies, newBook.numBorrowed);

    fclose(BookPtr);
    printf("Book added successfully to book.dat\n");
}

//Function to update student file every time a change is done
void updateStudentFile(student *updatedStudent) {
    FILE *originalFile = fopen("student.txt", "r");
    FILE *tempFile = fopen("temp_student.txt", "w");

    if (originalFile == NULL || tempFile == NULL) {
        printf("Error opening files for update.\n");
        return;
    }

    char line[MAX_COPY];
    char originalLine[MAX_COPY];
    int found = 0;

    while (fgets(line, sizeof(line), originalFile)) {
        strcpy(originalLine, line); // Keep a copy of the original line
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        char *token = strtok(line, "|");
        if (strcmp(token, updatedStudent->studentID) == 0) {
            // Update the line with the new student information
            fprintf(tempFile, "%s|%s|%d|", updatedStudent->studentID, updatedStudent->stdName, updatedStudent->numOfBooks);
            // Update borrowed books
            for (int i = 0; i < MAX_BOOKS; ++i) {
                fprintf(tempFile, "%s,%ld,", updatedStudent->time_since_borrowed[i].ISBN, updatedStudent->time_since_borrowed[i].borrowTime);
            }
            fprintf(tempFile, "%d|\n", updatedStudent->accessRight);
            found = 1; // Set the flag indicating the student record was found and updated
        } else {
            fprintf(tempFile, "%s", originalLine); // Write the original line to the file
        }
    }

    fclose(originalFile);
    fclose(originalFile);
    fclose(tempFile);
    fclose(tempFile);

    // Replace the original file with the updated content from the temporary file
    remove("student.txt"); // Delete the original file
    rename("temp_student.txt", "student.txt"); // Rename the temporary file to the original name
}

//Similar to update student file but for books
void updateBookFile(const book *updatedBook) {
    FILE *originalFile = fopen("book.txt", "r");
    FILE *tempFile = fopen("temp_book.txt", "w");

    if (originalFile == NULL || tempFile == NULL) {
        printf("Error opening files for update.\n");
        fclose(originalFile);
        fclose(tempFile);
        return;
    }

    char line[MAX_COPY];
    char originalLine[MAX_COPY];

    while (fgets(line, sizeof(line), originalFile)) {
        strcpy(originalLine, line); // Keep a copy of the original line
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        char *token = strtok(line, "|");
        if (strcmp(token, updatedBook->ISBN) == 0) {
            // Update the line with the new book information
            if (fprintf(tempFile, "%s|%s|%s|%s|%d|%d\n",
                    updatedBook->ISBN, updatedBook->Title, updatedBook->author,
                    updatedBook->publication_year, updatedBook->numCopies, updatedBook->numBorrowed) < 0) {
                printf("Error writing to file.\n");
                fclose(originalFile);
                fclose(tempFile);
                return;
            }
        } else {
            if (fprintf(tempFile, "%s", originalLine) < 0) { // Write the original line to the file
                printf("Error writing to file.\n");
                fclose(originalFile);
                fclose(tempFile);
                return;
            }
        }
    }

    fclose(originalFile);
    fclose(tempFile);

    // Replace the original file with the updated content from the temporary file
    if (remove("book.txt") != 0) { // Delete the original file
        perror("Error deleting file");
        return;
    }
    if (rename("temp_book.txt", "book.txt") != 0) { // Rename the temporary file to the original name
        perror("Error renaming file");
        return;
    }
}

// Function to borrow a book (only using ISBN)
void borrowBook(student *std, char *ISBN) {
    // Check if the student can borrow more books
    if (std->numOfBooks >= MAX_BOOKS) {
        printf("You've reached the maximum limit of books that can be borrowed.\n");
        return;
    }

    // Finding the book by ISBN in the book file
    book foundBook;

    foundBook = findBookByISBN(ISBN);
    if (strcmp(foundBook.ISBN , "") == 0){
        printf ("Book your are trying to borrow is not found\n");
        return;
    }

    if (foundBook.numBorrowed >= foundBook.numCopies){
        printf ("Book with ISBN %s is have no copies left \n", foundBook.ISBN);
        return;
    }

    // Book can be borrowed, update student's record
    strcpy(std->time_since_borrowed[std->numOfBooks].ISBN, ISBN);
    // Store the borrowing date
    std->time_since_borrowed[std->numOfBooks].borrowTime = time(NULL); // Store the current time
    std->numOfBooks++;

    // Initialize the remaining unassigned elements in the array
    for (int i = std->numOfBooks; i < MAX_BOOKS; ++i) {
        strcpy(std->time_since_borrowed[i].ISBN, " ");
        std->time_since_borrowed[i].borrowTime = 0;
    }

    // Update the student file after borrowing the book
    updateStudentFile(std);

    // Update the book's borrowed count
    foundBook.numBorrowed++;

    // Update the book file with the new borrowed count
    updateBookFile(&foundBook);
    
    //Append transaction detail
    appendTransactionDetails("borrow", ISBN, std->studentID, 0);

    printf("Book '%s' has been successfully borrowed. Please return book within %zu days.\n\n", foundBook.Title,MAX_DAYS);

}

// Function to calculate days elapsed since borrowing
int daysSinceBorrowed(time_t borrowTime) {
    time_t now;
    double seconds;

    // Get the current time
    time(&now);

    // Convert both times to seconds
    seconds = difftime(now, borrowTime);

    // Convert seconds to days
    int daysElapsed = seconds / (60 * 60 * 24);

    return daysElapsed;
}

// Function to return a book
void returnBook(student *std, char *ISBN) {
    int foundIndex = -1;

    // Find the book in the student's record and mark it as returned
    for (int i = 0; i < std->numOfBooks; ++i) {
        if (strcmp(std->time_since_borrowed[i].ISBN, ISBN) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Book with ISBN %s not found in the student's records.\n", ISBN);
        return;
    }

    // Calculate days elapsed since borrowing
    time_t returnTime = time(NULL);
    double seconds = difftime(returnTime, std->time_since_borrowed[foundIndex].borrowTime);
    int daysElapsed = (seconds / (60 * 60 * 24)) - MAX_DAYS; // Convert seconds to days
    const int finePerDay = 1; //Rm 1 per day late
    int fine = 0;
    if (daysElapsed > 0) {
        fine = finePerDay * daysElapsed;
        printf("The book is %d days overdue. You owe a fine of Rm %d.\n", daysElapsed, fine);
    }

    // Get the found book by ISBN
    book returnedBook = findBookByISBN(ISBN);
    if (strcmp(returnedBook.ISBN, "") == 0) {
        printf("Book with ISBN %s not found in the library records.\n", ISBN);
        return;
    }

    // Update the book file with decreased borrowed count
    returnedBook.numBorrowed--; // Decrease the borrowed count by 1
    updateBookFile(&returnedBook);
    
for (int i = foundIndex; i < std->numOfBooks; ++i) {
    // Check if the next index is within bounds and if it contains valid data
    if (i + 1 < MAX_BOOKS && strlen(std->time_since_borrowed[i + 1].ISBN) > 0) {
        // If the next index is not empty, shift the element forward
        strcpy(std->time_since_borrowed[i].ISBN, std->time_since_borrowed[i + 1].ISBN);
        std->time_since_borrowed[i].borrowTime = std->time_since_borrowed[i + 1].borrowTime;
    } else {
        // Initialize remaining indices to empty values
        for (int j = i; j < MAX_BOOKS; j++) {
            strcpy(std->time_since_borrowed[j].ISBN, "");
            std->time_since_borrowed[j].borrowTime = 0;
        }
        break; // Break out of the loop after handling remaining elements
    }
}


    std->numOfBooks--;
    
    // Update the student file after returning the book
    updateStudentFile(std);

    //Append return details
    appendTransactionDetails("return", ISBN, std->studentID, fine);

    printf("Book with Title %s has been successfully returned.\n", returnedBook.Title);
}

//Update transaction file
void appendTransactionDetails(char *transactionType, char *ISBN, char *studentID, int fine) {
    FILE *file = fopen("transactions.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    time_t now;
    time(&now); // Get the current time

    fprintf(file, "%s|%s|%s|%ld|%d\n", transactionType, ISBN, studentID, now, fine);

    fclose(file);
}

     //"%[^|]|%[^|]|%[^|]|%ld|%d"
        // Parse the line into a Transaction struct
        // Parse the line into a Transaction struct
        //- %[^|] : This is a scanset. It tells sscanf to read as many characters as it can that are not a |
        //It stops reading when it encounters a | or the end of the string. The read characters are stored in the corresponding argument.
        // | : This is a literal character. It tells sscanf to expect a | at this point in the input string.

//Function to change the number of copies in file
void modifyBookCopies(char *ISBN) {
    // Find the book by ISBN
    book foundBook = findBookByISBN(ISBN);
    if (strcmp(foundBook.ISBN, "") == 0) {
        printf("Book with ISBN %s not found.\n", ISBN);
        return;
    }

    // Prompt the user about the current number of copies
    printf("Current number of copies for book with ISBN %s: %d\n", ISBN, foundBook.numCopies);

    // Ask for the new number of copies
    int newNumCopies;
    printf("Enter the new number of copies: ");
    scanf("%d", &newNumCopies);

    // Ensure that the new number of copies is not lower than the number borrowed
    if (newNumCopies < foundBook.numBorrowed) {
        printf("Error: The number of copies cannot be lower than the number borrowed.\n");
        return;
    }

    // Update the number of copies
    foundBook.numCopies = newNumCopies;

    // Update the book file with the new number of copies
    updateBookFile(&foundBook);

    printf("Number of copies for book with ISBN %s has been updated to %d.\n", ISBN, newNumCopies);
}

//Output monthly report based on time
void generateMonthlyReport(int month, int year) {
    FILE *file = fopen("transactions.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char line[MAX_COPY];
    Transaction transaction;
    int borrowedCount = 0;
    int returnedCount = 0;
    int notReturned = 0;
    int totalFine = 0;

    //Write data into struct from line
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%ld|%d", transaction.type, transaction.ISBN, transaction.studentID, &transaction.date, &transaction.fine);
        struct tm *transactionDate = localtime(&transaction.date);
        if (transactionDate->tm_mon + 1 == month && transactionDate->tm_year + 1900 == year) {
            if (strcmp(transaction.type, "borrow") == 0) {
                borrowedCount++;
            } else if (strcmp(transaction.type, "return") == 0) {
                returnedCount++;
                totalFine += transaction.fine;
            }
            notReturned = borrowedCount - returnedCount;
        }
    }

    fclose(file);

    printf("Monthly Report for %02d/%d\n", month, year);
    printf("Number of books borrowed: %d\n", borrowedCount);
    printf("Number of books returned: %d\n", returnedCount);
    printf("Number of books pending for return: %d\n", notReturned);
    printf("Total fines collected: %d\n", totalFine);
}


// strftime() function convert time stored in struct tm and then store into dateStr with format %c eg:(Sun Aug 19 02:56:02 2012)
// Reference: https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm

//The struct tm is a structure used to hold the time and date. It is defined in the <time.h> header file.
//The localtime function takes a pointer to a time_t object that represents calendar time and converts it to a broken-down time, expressed as a struct tm object. 
//The strftime function formats the time represented in the struct tm object into a string.


void displayStudentTransactions(const char *studentID, int month, int year) {
    // Open the transactions file in read mode
    FILE *file = fopen("transactions.txt", "r");
    if (file == NULL) {
        printf("Error opening transactions file.\n");
        return;
    }

    char line[MAX_LEN];
    Transaction transaction;
    // Create a list to store the borrowed books
    char borrowedBooks[MAX_BOOKS][MAX_LEN] = {0};
    time_t borrowedTimes[MAX_BOOKS] = {0};
    int borrowedCount = 0;
    int hasRecord = 0;

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        // Parse the line into a Transaction struct
        sscanf(line, "%[^|]|%[^|]|%[^|]|%ld|%d",
               transaction.type, transaction.ISBN, transaction.studentID, &transaction.date, &transaction.fine);

        // Convert the transaction date to a struct tm
        struct tm *transactionDate = localtime(&transaction.date);

        // If the transaction is for the given student and in the given month and year
        if (strcmp(transaction.studentID, studentID) == 0 && transactionDate->tm_mon + 1 == month && transactionDate->tm_year + 1900 == year) {
            hasRecord = 1;
            // Format the date into a string
            char dateStr[80];
            strftime(dateStr, 80, "%c", transactionDate);

            // Print the transaction details
            printf("Type: %s, ISBN: %s, Date: %s, Fine: %d\n",
                   transaction.type, transaction.ISBN, dateStr, transaction.fine);

            // If the transaction is a borrow, add the book to the list
            if (strcmp(transaction.type, "borrow") == 0) {
                strcpy(borrowedBooks[borrowedCount], transaction.ISBN);
                borrowedTimes[borrowedCount] = transaction.date;
                borrowedCount++;
            }
            // If the transaction is a return, remove the book from the list
            else if (strcmp(transaction.type, "return") == 0) {
                for (int i = 0; i < borrowedCount; i++) {
                    if (strcmp(borrowedBooks[i], transaction.ISBN) == 0) {
                        // Shift all elements to the left (Essentially removing one element from both of the arrays)
                        for (int j = i; j < borrowedCount - 1; j++) {
                            strcpy(borrowedBooks[j], borrowedBooks[j + 1]);
                            borrowedTimes[j] = borrowedTimes[j + 1];
                        }
                        borrowedCount--;
                        break;
                    }
                }
            }
        }
    }

    fclose(file);

    if (!hasRecord) {
        printf("No record for student %s in %02d/%d.\n", studentID, month, year);
        return;
    }

    // Display the borrowed books that have not been returned
    
    if (borrowedCount != 0){
        printf("\nBooks not returned:\n");
        for (int i = 0; i < borrowedCount; i++) {
            // Convert the borrow time to a struct tm
            struct tm *timeinfo = localtime(&borrowedTimes[i]);
            // Format the borrow time into a string
            char dateStr[80];
            strftime(dateStr, 80, "%c", timeinfo);
            // Print the book ISBN and borrow date
            printf("ISBN: %s, Borrow Date: %s\n", borrowedBooks[i], dateStr);
        }
    }
    else {
        printf("\nBooks not returned: NULL\n");
    }
}

//Note function will not check for books that are not returned for the past month
void displayBookTransactions(const char *ISBN, int month, int year) {
    // Open the transactions file in read mode
    FILE *file = fopen("transactions.txt", "r");
    if (file == NULL) {
        printf("Error opening transactions file.\n");
        return;
    }

    char line[MAX_LEN];
    Transaction transaction;
    // Create a list to store the borrowed students
    char borrowedStudents[MAX_BOOKS][MAX_LEN] = {0};
    time_t borrowedTimes[MAX_BOOKS] = {0};
    int borrowedCount = 0;
    int hasRecord = 0;

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%ld|%d",
               transaction.type, transaction.ISBN, transaction.studentID, &transaction.date, &transaction.fine);

        // Convert the transaction date to a struct tm
        struct tm *transactionDate = localtime(&transaction.date);

        // If the transaction is for the given book and in the given month and year
        if (strcmp(transaction.ISBN, ISBN) == 0 && transactionDate->tm_mon + 1 == month && transactionDate->tm_year + 1900 == year) {
            hasRecord = 1;
            // Format the date into a string
            char dateStr[80];
            strftime(dateStr, 80, "%c", transactionDate);

            // Print the transaction details
            printf("Type: %s, Student ID: %s, Date: %s, Fine: %d\n",
                   transaction.type, transaction.studentID, dateStr, transaction.fine);

            // If the transaction is a borrow, add the student to the list
            if (strcmp(transaction.type, "borrow") == 0) {
                strcpy(borrowedStudents[borrowedCount], transaction.studentID);
                borrowedTimes[borrowedCount] = transaction.date;
                borrowedCount++;
            }
            // If the transaction is a return, remove the student from the list
            else if (strcmp(transaction.type, "return") == 0) {
                for (int i = 0; i < borrowedCount; i++) {
                    if (strcmp(borrowedStudents[i], transaction.studentID) == 0) {
                        // Shift all elements to the left (Essentially removing one element from both of the arrays)
                        for (int j = i; j < borrowedCount - 1; j++) {
                            strcpy(borrowedStudents[j], borrowedStudents[j + 1]);
                            borrowedTimes[j] = borrowedTimes[j + 1];
                        }
                        borrowedCount--;
                        break;
                    }
                }
            }
        }
    }

    fclose(file);

    if (!hasRecord) {
        printf("No record for book %s in %02d/%d.\n", ISBN, month, year);
        return;
    }

    // Display the students who have not returned the book
    if (borrowedCount != 0){
        printf("\nStudents who have not returned the book:\n");
        for (int i = 0; i < borrowedCount; i++) {
            // Convert the borrow time to a struct tm
            struct tm *timeinfo = localtime(&borrowedTimes[i]);
            // Format the borrow time into a string
            char dateStr[80];
            strftime(dateStr, 80, "%c", timeinfo);
            // Print the student ID and borrow date
            printf("Student ID: %s, Borrow Date: %s\n", borrowedStudents[i], dateStr);
        }
    }
    else {
        printf("\nStudents who have not returned the book: NULL\n");
    }
}


int main() {
    char input[MAX_LEN];
    while (1) {
        printf("Enter your student ID or 'exit' to quit: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character

        if (strcmp(input, "exit") == 0) {
            break; // Exit the loop and end the program
        }

        student foundStudent = findStudentByID(input);
        if (strcmp(foundStudent.studentID, "") == 0) {
            printf("Student with ID %s not found.\n", input);
            continue;
        }

        if (foundStudent.accessRight == 2) {
            // Prompt for password and check it
            char password[MAX_LEN];
            printf("Enter your password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0'; // Remove newline character

            if (strcmp(password, "librarian") == 0) {
                // Access librarian functions
                int UInput;
                do{
                printf ("\nLIBRARIAN ACCESS: WELCOME %s\n",foundStudent.stdName);
                printSeparater(50);

                //Print current student details
                puts("Details:");
                printSeparater(40);
                printStudentDetails(&foundStudent);
                printSeparater(40);

                puts("Please select from the following function");
                puts("1. Search Books");
                puts("2. Borrow Books");
                puts("3. Return Books");
                puts("4. Display All Books");
                puts("5. Generate Monthly Report");
                puts("6. Borrow & Return Information");
                puts("7. Modify File");
                puts("8. Log off");

                puts("");

                printf ("Your Selection: ");
                UInput= getValidInteger(); 
                puts("");
                switch (UInput) {
                    int flag;
                    case 1:
                        flag = 1;
                        while (flag) {
                            puts ("\nSEARCH BOOKS:");
                            printSeparater(30);
                            puts("");
                            puts("1.Search by ISBN");
                            puts("2.Search by Title");
                            puts ("3. Exit to Main Menu");
                            printf("Your Selection: ");
                            int searchInput = getValidInteger();
                            if (searchInput == 1){
                                puts("\nSearch by ISBN Selected");
                                printSeparater(40);
                                char ISBN[MAX_LEN];
                                getISBN(ISBN);
                                book foundBook = findBookByISBN(ISBN);
                                printBookStruct (foundBook);
                            }
                            else if (searchInput == 2){
                                puts("\nSearch by Title Selected");
                                printSeparater(40);
                                puts("Enter Title");
                                char Title[MAX_LEN];
                                fgets(Title, sizeof(Title),stdin);
                                Title[strcspn(Title, "\n")] = '\0'; // Remove newline character
                                SearchResult allFoundBook = searchBookByName(Title);
                                printSimilarBooks (allFoundBook);
                            }
                            else if (searchInput == 3){
                                puts("Exit to Main Menu");
                                flag = 0;
                            }
                            else {
                                puts("Invalid selection");
                            }
                        }
                        flag = 1;
                        break;
                    case 2:
                        // Call function to borrow books
                        while(flag){
                            puts ("Borrow BOOKS:");
                            printSeparater(30);
                            puts("1.Borrow by ISBN");
                            puts("2.Exit to Main Menu\n");
                            printf("Your Selection: ");
                            int borrowInput = getValidInteger();
                            if (borrowInput == 1){
                                puts ("\nBorrow Book by ISBN Selected");
                                char ISBN[MAX_LEN];
                                getISBN(ISBN);
                                borrowBook(&foundStudent,ISBN);
                            }
                            else if (borrowInput == 2){
                                puts("Exit to Main Menu");
                                flag = 0;
                            }
                            else {
                                puts("Invalid Selection");
                            }
                        }
                        flag = 1;
                        break;
                    case 3:
                        // Call function to return books

                        while(flag){
                            puts ("\nReturn BOOKS:");
                            printSeparater(30);
                            puts("1.Return book by ISBN");
                            puts("2.Exit to Main Menu\n");
                            printf("Your Selection: ");
                            int returnInput  = getValidInteger();
                            if (returnInput  == 1){
                                puts ("\nReturn Book by ISBN Selected");
                                char ISBN[MAX_LEN];
                                getISBN(ISBN);
                                returnBook(&foundStudent,ISBN);
                            }
                            else if (returnInput == 2){
                                puts("Exit to Main Menu");
                                flag = 0;
                            }
                            else {
                                puts("Invalid Selection");
                            }
                        }
                        flag = 1;
                        break;
                    case 4:
                        // Call function to display all books
                        while (flag){
                            displayAllBooks();
                            flag = 0;
                        }
                        flag = 1;
                        break;
                    case 5:
                        // Call function to generate monthly report
                        while(flag){
                            puts("Enter the month for your monthly report in the form m yyyy:");
                            int year, month;
                            scanf("%d %d" , &month,&year);
                            generateMonthlyReport(month,year);
                            flag = 0;
                        }
                        flag = 1;
                        break;
                    case 6:
                        // Call function to display borrow & return information
                        while (flag){
                            puts("Enter the month for your monthly report in the form m yyyy:");
                            int year, month;
                            scanf("%d %d" , &month,&year);

                            puts ("1.Display all student borrow and return information:");
                            puts ("2.Display all books borrow and return information:\n");

                            printf("Your Selection: ");
                            int displayInput = getValidInteger();

                            if (displayInput == 1){
                                puts ("Enter the student's ID you wish to search");
                                char studentID [MAX_LEN];
                                fgets(studentID,MAX_LEN,stdin);
                                studentID[strcspn(studentID,"\n")] = '\0'; //Remove newline
                                puts("Displaying info");
                                printSeparater(50);
                                displayStudentTransactions(studentID,month,year);
                                flag = 0;
                            }
                            else if (displayInput == 2){
                                puts ("Enter the ISBN you wish to search");
                                char ISBN [MAX_LEN];
                                getISBN(ISBN);
                                puts("Displaying info");
                                printSeparater(50);
                                displayBookTransactions(ISBN,month,year);
                                flag = 0;
                            }
                        }
                        flag = 1;   
                        break;
                    
                    case 7:
                        while (flag){
                            puts("Modify file selected");
                            printSeparater(30);
                            puts("1. Add new books");
                            puts("2. Modify number of books");
                            puts("3. Add New Students");
                            puts("4. Exit to main menu");
                            printf("\n Your Selection: ");
                            int modInput = getValidInteger();
                            if (modInput == 1){
                                addBookToFile();
                            }
                            else if (modInput == 2){
                                puts ("Enter the ISBN for book you wish to modify: ");
                                char ISBN [MAX_LEN];
                                getISBN(ISBN);
                                modifyBookCopies(ISBN);

                            }
                            else if (modInput == 3){
                                addStudentToFile();                             
                            }
                            else if (modInput == 4){                            
                                flag = 0;
                            }
                        }
                        flag = 1;
                        break;

                    case 8:
                        // Log off
                        break;
                    default:
                        printf("Invalid option. Please enter a number between 1 and 8.\n");
                        flag = 1;
                        break;
                }
            }while (UInput != 8);

                } else {
                    printf("Incorrect password.\n");
                }
            } else {
            //Student Access
                int UInput;

                do{
                printf("\nSTUDENT ACCESS: WELCOME %s\n",foundStudent.stdName);
                printSeparater(50);
                //printing current details
                puts("Details:");
                printSeparater(50);
                printStudentDetails(&foundStudent);
                printSeparater(50);

                puts("Please select from the following function");
                puts("1. Search Books");
                puts("2. Borrow Books");
                puts("3. Return Books");
                puts("4. Display All Books");
                puts("5. Log off");

                puts("");

                printf ("Your Selection: ");
                UInput= getValidInteger(); 
                puts("");
                switch (UInput) {
                    int flag;
                    case 1:
                        flag = 1;
                        while (flag) {
                            puts ("\nSEARCH BOOKS:");
                            printSeparater(30);
                            puts("");
                            puts("1.Search by ISBN");
                            puts("2.Search by Title");
                            puts ("3. Exit to Main Menu\n");
                            printf("Your Selection: ");
                            int searchInput = getValidInteger();
                            if (searchInput == 1){
                                puts("\nSearch by ISBN Selected");
                                printSeparater(40);
                                char ISBN[MAX_LEN];
                                getISBN(ISBN);
                                book foundBook = findBookByISBN(ISBN);
                                printBookStruct (foundBook);
                            }
                            else if (searchInput == 2){
                                puts("\nSearch by Title Selected");
                                printSeparater(40);
                                puts("Enter Title");
                                char Title[MAX_LEN];
                                fgets(Title, sizeof(Title),stdin);
                                Title[strcspn(Title, "\n")] = '\0'; // Remove newline character
                                SearchResult allFoundBook = searchBookByName(Title);
                                printSimilarBooks (allFoundBook);
                            }
                            else if (searchInput == 3){
                                puts("Exit to Main Menu");
                                flag = 0;
                            }
                            else {
                                puts("Invalid selection");
                            }
                        }
                        flag = 1;
                        break;
                    case 2:
                        // Call function to borrow books
                        while(flag){
                            puts ("Borrow BOOKS:");
                            printSeparater(30);
                            puts("1.Borrow by ISBN");
                            puts("2.Exit to Main Menu\n");
                            printf("Your Selection: ");
                            int borrowInput = getValidInteger();
                            if (borrowInput == 1){
                                puts ("Borrow Book by ISBN Selected");
                                char ISBN[MAX_LEN];
                                getISBN(ISBN);
                                borrowBook(&foundStudent,ISBN);
                            }
                            else if (borrowInput == 2){
                                puts("Exit to Main Menu");
                                flag = 0;
                            }
                            else {
                                puts("Invalid Selection");
                            }
                        }
                        flag = 1;
                        break;
                    case 3:
                        // Call function to return books
                        while(flag){
                            puts ("Return BOOKS:");
                            printSeparater(30);
                            puts("1.Return book by ISBN");
                            puts("2.Exit to Main Menu\n");
                            printf("Your Selection: ");
                            int returnInput  = getValidInteger();
                            if (returnInput  == 1){
                                puts ("Return Book by ISBN Selected");
                                char ISBN[MAX_LEN];
                                getISBN(ISBN);
                                returnBook(&foundStudent,ISBN);
                            }
                            else if (returnInput == 2){
                                puts("Exit to Main Menu");
                                flag = 0;
                            }
                            else {
                                puts("Invalid Selection");
                            }
                        }
                        flag = 1;
                        break;
                    case 4:
                        // Call function to display all books
                        while (flag){
                            displayAllBooks();
                            flag = 0;
                        }
                        flag = 1;
                        break;
                    case 5:
                        // Log off
                        break;
                    default:
                        printf("Invalid option. Please enter a number between 1 and 5.\n");
                        flag = 1;
                        break;
                }
            }while (UInput != 5);

        }
    }

    return 0;
}

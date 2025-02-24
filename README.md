# Library Management System

## Overview
This system provides a user-friendly interface for managing library transactions, supporting both students and librarians. It allows searching, borrowing, returning books, and maintaining transaction logs.

## Important Notes
- To test the penalty system, change your Windows time, borrow a book, and revert to the current time before returning it.
- Relaunch the program after modifying system time.
- **DO NOT DELETE** any of the following `.txt` files:
  - `book.txt`
  - `student.txt`
  - `transaction.txt`
- If deleted, re-create empty files with the corresponding names.
- `transaction.txt` should generally not be modified and was left empty when handing in coursework.

## File System Structure
- **`book.txt`**: Stores book details in `ISBN|TITLE|AUTHOR_NAME|PUBLICATION_DATE|NUMBER_OF_COPIES_AVAILABLE|COPIES_BORROWED` format.
- **`student.txt`**: Stores student data in `StudentID|Name|Num_Book_Borrowed|Array,AccessLevel|` format.
  - The array stores ISBNs of borrowed books and time since borrowed, separated by commas.
  - The last element in the array and access level are also separated by a comma instead of a pipe delimiter.
  - **Time format**: Epoch time (LocalTime). Example: `1702010341 = Friday, 8 December 2023 04:39:01`
- **`transaction.txt`**: Logs transactions in `TYPE_OF_TRANSACTION|ISBN|STUDENT_ID|TIME_OF_TRANSACTION|FINE_RECEIVED|` format.

## User Roles
### Student
1. **Search Books** - By ISBN or title (case-sensitive and exact ISBN format required).
2. **Borrow Books** - Enter ISBN to borrow a book.
3. **Return Books** - Enter ISBN to return a book.
4. **Display All Books** - View all available books.
5. **Log Off** - Exit the system.

### Librarian
1. **All Student Functions**
2. **Generate Monthly Report** - Enter a month and year to view transactions.
3. **Borrow & Return Info** - View details by student ID or ISBN.
4. **Modify File** - Add new books, modify copies, or add students.
5. **Log Off**

## Handling Borrowing & Returning
- Books are updated via temporary files to prevent data loss.
- `MAX_COPY` defines how many lines can be copied at once.
- `MAX_DAYS` determines the borrowing time limit.
- Returning books follows FIFO if multiple copies are borrowed.
- When returning identical books, the one borrowed first will be returned first.
- The system will warn if a book has not been returned from a past month, as it may not be displayed in monthly logs.

## Testing Penalty System
- Borrow books after changing system time.
- Return them in the current time to observe penalties.
- Past month transactions are not displayed in borrow/return logs.

## How to Run the System
1. Ensure all `.txt` files exist.
2. Launch the program and enter your Student ID.
3. Follow the menu options based on your role.
4. To generate Gantt charts for transaction logs, use appropriate scripts.

## Notes on Modifying Files
- Ensure file pointers are set correctly before modifying `.txt` files.
- **Epoch Time Format** is used for timestamps.
- Incorrect modifications may lead to data inconsistencies.
- When modifying `book.txt` or `student.txt`, ensure:
  - The file structure is maintained.
  - Student ID and ISBN remain unique.
  - The number of book copies is never reduced below the number currently borrowed.

## Author
Developed by [Leong Jun Chuen].
Student of The University of Nottingham Malayisa

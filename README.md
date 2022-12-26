# Simple C student points database

# Assignment
Implement a system that tracks the exercise points for a course. The course has six exercise rounds, for which points are tracked. For each student registered to the course, the system is required to store

- student number, which is a string that can contain at most 6 characters.
- first name, which is a string that can have arbitrary length.
- last name, which is a string that can have arbitrary length.

The program should be able to support arbitrary number of students.



## Commands
- A [student number] [last name] [first name] --- add student
- U [student number] [round] [points]         --- update points
- L                                           --- list current students and points
- W [filename]                                --- write to plaintext file
- O [filename]                                --- write from plaintext file
- Q                                           --- quit

ASSUMPTION: arguments don't contain spaces

student number: max. 6 digits
round: between 1 and 6
points: nonnegative integers
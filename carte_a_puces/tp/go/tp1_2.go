package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"time"
)

func add(a *uint32, b *uint32, c *uint32) {
	*a = *b + *c
}
func sub(a *uint32, b *uint32, c *uint32) {
	*a = *b - *c
}
func mul(a *uint32, b *uint32, c *uint32) {
	*a = *b * *c
}
func div(a *uint32, b *uint32, c *uint32) {
	*a = *b / *c
}
func and(a *uint32, b *uint32, c *uint32) {
	*a = *b & *c
}
func or(a *uint32, b *uint32, c *uint32) {
	*a = *b | *c
}
func xor(a *uint32, b *uint32, c *uint32) {
	*a = *b ^ *c
}
func addi(a *uint32, b *uint32, c uint32) {
	*a = *b + c
}
func subi(a *uint32, b *uint32, c uint32) {
	*a = *b - c
}
func muli(a *uint32, b *uint32, c uint32) {
	*a = *b * c
}
func divi(a *uint32, b *uint32, c uint32) {
	*a = *b / c
}
func andi(a *uint32, b *uint32, c uint32) {
	*a = *b & c
}
func ori(a *uint32, b *uint32, c uint32) {
	*a = *b | c
}
func xori(a *uint32, b *uint32, c uint32) {
	*a = *b ^ c
}
func beq(a *uint32, b *uint32, c uint32, index *uint32) {
	if *a == *b {
		*index = c
	} else {
		*index += 1
	}
}
func bne(a *uint32, b *uint32, c uint32, index *uint32) {
	if *a != *b {
		*index = c
	} else {
		*index += 1
	}
}
func blo(a *uint32, b *uint32, c uint32, index *uint32) {

	if *a < *b {
		*index = c
	} else {
		*index += 1
	}
}
func bgt(a *uint32, b *uint32, c uint32, index *uint32) {
	if *a > *b {
		*index = c
	} else {
		*index += 1
	}
}
func lw(a *uint32, b *uint32, c uint32, ram []uint32) {
	*a = ram[*b+c]
}
func sw(a *uint32, b *uint32, c uint32, ram []uint32) {
	ram[*b+c] = *a
}

var instr_code_pointer_arith = map[uint32](func(*uint32, *uint32, *uint32)){
	0: add,
	1: sub,
	2: mul,
	3: div,
	4: and,
	5: or,
	6: xor,
}

var instr_code_arith = map[uint32](func(*uint32, *uint32, uint32)){
	7:  addi,
	8:  subi,
	9:  muli,
	10: divi,
	11: andi,
	12: ori,
	13: xori,
}
var instr_code_compar = map[uint32](func(*uint32, *uint32, uint32, *uint32)){
	14: beq,
	15: bne,
	16: blo,
	17: bgt,
}

var instr_code_memory = map[uint32](func(*uint32, *uint32, uint32, []uint32)){
	18: lw,
	19: sw,
}

func process_file(filename string) ([]uint32, int) {
	f, err := os.Open(filename)
	if err != nil {
		log.Fatal(err)
	}

	len_file, _ := f.Seek(0, 2)
	f.Seek(0, 0)

	text := make([]byte, len_file)

	_, err = f.Read(text)
	if err != nil {
		log.Fatal(err)
	}
	f.Close()

	lines := strings.Split(bytes.NewBuffer(text).String(), "\n")
	nb_lines := len(lines)

	instruction := make([]uint32, nb_lines)
	for i, line := range lines {
		if i == nb_lines-1 {
			break
		}
		tmp, err := strconv.ParseUint(line, 2, 32)
		if err != nil {
			log.Fatal(err)
		}
		instruction[i] = uint32(tmp)
	}

	return instruction, nb_lines
}

var instr_pos = map[int]uint32{
	0: 24,
	1: 18,
	2: 12,
	3: 0,
}

var instr_size = map[int]uint32{
	0: 31,
	1: 63,
	2: 63,
	3: 4095,
}

func parse_coded_instr(instruction uint32) []uint32 {
	decoded_instr := make([]uint32, 4)
	for i := 0; i < 4; i += 1 {
		decoded_instr[i] = (instruction >> instr_pos[i]) & instr_size[i]
	}
	return decoded_instr
}
func main() {
	instruction_sheet, nb_instructions := process_file("coded.txt")
	fmt.Println(instruction_sheet)
	registres := make([]uint32, 64)

	// 64 + 2**12 < 2**13 ducoup on adresse sur 13 bits
	ram := make([]uint32, 1<<12)

	index := uint32(0)
	for index < uint32(nb_instructions) {
		time.Sleep(50000000)
		instruction := parse_coded_instr(instruction_sheet[index])

		switch {
		case 0 <= instruction[0] && instruction[0] < 7:
			instr_code_pointer_arith[instruction[0]](&registres[instruction[1]],
				&registres[instruction[2]], &registres[instruction[3]])

		case 7 <= instruction[0] && instruction[0] < 14:
			instr_code_arith[instruction[0]](&registres[instruction[1]],
				&registres[instruction[2]], instruction[3])

		case 14 <= instruction[0] && instruction[0] < 18:
			instr_code_compar[instruction[0]](&registres[instruction[1]],
				&registres[instruction[2]], instruction[3], &index)
			registres[0] = uint32(0)
			fmt.Println("registres :", registres[:32])
			fmt.Println("ram :", ram[:32])
			fmt.Printf("\n")
			continue

		case 18 <= instruction[0] && instruction[0] < 20:
			instr_code_memory[instruction[0]](&registres[instruction[1]],
				&registres[instruction[2]], instruction[3], ram)

		}

		registres[0] = uint32(0)
		index += 1
		fmt.Println("registres :", registres[:32])
		fmt.Println("ram :", ram[:32])
		fmt.Printf("\n")
	}

}

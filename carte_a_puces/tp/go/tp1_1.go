package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

var instr_code = map[string]uint32{
	"add":  0,
	"sub":  1,
	"mul":  2,
	"div":  3,
	"and":  4,
	"or":   5,
	"xor":  6,
	"addi": 7,
	"subi": 8,
	"muli": 9,
	"divi": 10,
	"andi": 11,
	"ori":  12,
	"xori": 13,
	"beq":  14,
	"bne":  15,
	"blo":  16,
	"bgt":  17,
	"lw":   18,
	"sw":   19,
}

/*On decale de combien le code associé au numéro d'instruction*/
var instr_encode = map[int8]int8{
	0: 24,
	1: 18,
	2: 12,
	3: 0,
	//Pour pas avoir d'out of range
	4: 0,
}

func process_file(filename string) ([]string, int) {
	f, err := os.Open("to_parse.txt")
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

	return lines, nb_lines
}

/*
On entre dans cette fonction au moment de lire la première instruction d'une ligne
Marche en suivant le codage donné par instr_code
Retourne le code de l'instruction ainsi que son type pour parser plus facilement
*/
func parse_func(instr string) (uint32, int8) {
	n, verif := instr_code[instr]
	if !verif {
		log.Fatal("instruction inconnue:", instr)
	}
	switch {
	case 0 <= n && n <= 13:
		return n, 1
	case 13 < n && n <= 17:
		return n, 2
	case 17 < n && n <= 19:
		return n, 3
	}

	fmt.Println("ne devrait pas arriver")
	return 1 << 31, 1 << 6
}

/*
Fonction envoyée dans strings.Map utilisée en
interne dans parse_instr.

	Remplace tout les caractères non numériques par des espaces
*/
func invalid_chars(r rune) rune {
	switch {
	case 0 <= r-'0' && r-'0' < 10:
		return r
	default:
		return ' '
	}
}

/*instruction arithmétique : 1*/
/*instruction de comparaison : 2*/
/*instruction mémoire*/

/*
Etant donné le numéro et le type d'instruction suivant
instr_code,  renvoie le code associé à l'instruction. Dans le cas
ou l'instruction est load ou save new_instr
contient deux éléments
*/
func parse_instr(instr string, instr_nb int8, instr_type int8,
	labels map[string]int) (uint32, uint32) {

	new_instr := []string{}
	if instr_nb == 3 && instr_type == 2 {
		/*Cas ou on a un label*/
		new_instr = []string{instr}
	} else {
		/*
			Autre cas : Si c'est une opération mémoire "32($12)" devient "32 12"
			et on obtient un tableau à deux éléments. De meme
			"$12," devient "12" et on a un tableau a un élément
		*/
		new_instr = strings.Split(strings.TrimSpace(strings.Map(invalid_chars, instr)), " ")
	}

	/*
		traitement différent dans le cas d'opération
		1 : arithmétique,
		2 : de comparaison,
		3 : mémoire
	*/
	switch instr_type {
	case 1:
		/*A ce stade new_instr est juste un nombre */
		code, err := strconv.Atoi(new_instr[0])
		if err != nil {
			log.Fatal(err)
		}
		return uint32(code), uint32(0)
	case 2:
		switch instr_nb {
		case 1:
			/*A ce stade new_instr est juste un nombre */
			code, err := strconv.Atoi(new_instr[0])
			if err != nil {
				log.Fatal(err)
			}
			return uint32(code), uint32(0)
		case 2:
			/*A ce stade new_instr est juste un nombre */
			code, err := strconv.Atoi(new_instr[0])
			if err != nil {
				log.Fatal(err)
			}
			return uint32(code), uint32(0)
		case 3:
			/*Ici faut gérer les labels*/
			n, ok := labels[instr]
			if ok {
				/*
					n contient la ligne de label a laquelle on
					soustrait le nombre de label le précedent.
					Le but étant qu'au moment de la lecture le
					code de label pointe sur la ligne le suivant.
				*/
				return uint32(n), uint32(0)
			} else {
				log.Fatal("ne trouve pas le label, ne devrait pas arriver")
			}
		}
	case 3:
		/*
			A ce stade new_instr contient un tableau
			a deux éléments. Chacun ayant une string
			représentant un entier
		*/
		switch instr_nb {
		case 1:
			/*A ce stade new_instr est juste un nombre */
			code, err := strconv.Atoi(new_instr[0])
			if err != nil {
				log.Fatal(err)
			}
			return uint32(code), uint32(0)
		case 2:
			code1, err := strconv.Atoi(new_instr[0])
			if err != nil {
				log.Fatal(err)
			}

			code2, err := strconv.Atoi(new_instr[1])
			return uint32(code2), uint32(code1)
		}

		/*On a déja tout renvoyé à l'étape 2*/
		fmt.Println("cas 3, ne devrait pas arriver")
		return uint32(1 << 31), uint32(1 << 31)
	}

	fmt.Println("parse_instr, ne devrait pas arriver")
	return uint32(0), uint32(0)

}

func parse_line(line_of_instr []string, labels map[string]int) uint32 {
	coded, tmp1, tmp2, op_type := uint32(0), uint32(0), uint32(0), int8(0)

	line_len := len(line_of_instr)
	for i, instr := range line_of_instr {

		if i == line_len {
			break
		}
		if i == 0 {
			tmp1, op_type = parse_func(instr)
			coded = tmp1 << 24
		} else if op_type == int8(3) && i == 3 {
			continue
		} else {
			tmp1, tmp2 = parse_instr(instr, int8(i), op_type, labels)

			/*Pour gérer d'un coup tout les cas, sauf pour op_type=3 : tmp2 = 0*/
			coded += tmp1<<instr_encode[int8(i)] + tmp2<<instr_encode[int8(i)+1]
		}
	}

	return coded
}

func main() {

	lines, nb_lines := process_file("to_parse.txt")
	nb_lines -= 1

	coded_instructions := make([]uint32, nb_lines)
	labels := make(map[string]int, nb_lines)

	/*
		Premier parcourt du tableau de lignes
		pour repérer les labels. Si line est de la forme
		"label:". On enleve les : et on ajoute
		label a la map : labels[label] = ligne de l'instruction.
		A chaque label on pose coded_instructions[index] = -1
	*/
	for n, line := range lines {
		if nb_lines != n && string(line[0]) != " " {
			/*
				labels[label] contient le numéro de
				la ligne après label en ayant soustrait
				tout les labels précendents.
				(le code du jeu d'instruction ne contient pas les labels)
			*/
			line := strings.Trim(line, ":")
			labels[line] = n - len(labels)
			coded_instructions[n] = uint32(1 << 31)

		}
	}

	/*Deuxieme parcours du tableau ou cette fois
	on parse les lignes ne contenant par de label.
	coded_instruction[index] prend alors comme valeur
	un entier sur 32 bits code tel que :
		-code>>24 contient l'opération sur 5 bits
		-(code>>18)&64 contient la premiere entrée sur 6 bits
		-(code>>12)&64 contient la deuxieme entrée sur 6 bits
		-(code)&(4096) contient la troisième entrée sur 12 bits
	*/
	for n, line := range lines {
		if n == nb_lines {
			break
		}
		if nb_lines != n && string(line[0]) != " " {
			continue
		} else {

			/*

			 */
			line_of_instr := strings.Split(strings.Trim(line, " "), " ")

			coded_instructions[n] = parse_line(line_of_instr, labels)
		}
	}

	f, err := os.Create("coded.txt")
	if err != nil {
		log.Fatal(err)
	}

	for _, instruction := range coded_instructions {
		if instruction != 1<<31 {
			fmt.Fprintf(f, "%032b\n", instruction)
		}
	}

}

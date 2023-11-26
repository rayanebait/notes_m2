# 1.

Corrigé au tableau

# 2.

OCR0A est compris entre 0 et 255

sans inversement :
- Si OCROA = BOTTOM = 0, alors duty cycle = 0 (le niveau est toujours low)
- Si OCROA = MAX = 255, alors duty cycle = 1 (le niveau est toujours high)
- Si OCR0A = MAX / 2 = 127, alors duty cycle = 0.5

duty cycle = 0.25 => OCR0A = 64
duty cycle = 0.75 => OCR0A = 191


Voir le schéma de la doc avec la valeur de TCNT0 qui s'incrémente et la
valeur de OCOA qui évolue.

# 3.

OC0A est sur PD6

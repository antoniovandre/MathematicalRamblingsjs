// Proprietário: Antonio Vandré Pedrosa Furtunato Gomes (bit.ly/antoniovandre_legadoontologico).

// Projeto Mathematical Ramblings (bit.ly/mathematicalramblings_github).

// Biblioteca C de funções e constantes pertencentes a Antonio Vandré Pedrosa Furtunato Gomes, versão de distribuição para antoniovandre_eval GUI.

// Licença de uso: Atribuição-NãoComercial-CompartilhaIgual (CC BY-NC-SA).

// Última atualização: 12-10-2020.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define __USE_GNU
#include <math.h>
#include <float.h>

#include "antoniovandre_constantes.c"

#include "antoniovandre_extra.c"

#define TAMANHO_BUFFER_SMALL 75 // Para pequenos buffers.
#define TAMANHO_BUFFER_WORD 8192 // Para strings pequenas.
#define TAMANHO_BUFFER_PHRASE 81920 // Para strings grandes.
#define VALOR_MAX 1000000000L // Afim de evitar erros de saída.
#define TAMANHO_MAX_ARQUIVO 1000000000000 // Afim de evitar erros de saída.
#define OPERADORMULTIPLICACAO '*'
#define DELIMITADORSTRING ',' // Deve ser um char;
#define DELIMITADORSTRING2 ';' // Deve ser um char, diferente de DELIMITADORSTRING;
#define STRINGSAIDAERRO "Erro de saida de uma string."
#define STRINGSAIDAERROOVER "Erro de saida de uma string por over."
#define MENSAGEM_ERRO_OVER "Um dos números atingiu o limite máximo."
#define TENTATIVASLOGICAS 4 // Podem ser necessárias mais de uma verificação lógica em alguns trechos.
#define TOKENINICIOEVAL '('
#define TOKENFIMEVAL ')'
#define EPSILON 0.001 // Para funções de Cálculo Diferencial.
#define VARIAVELDESUBSTITUICAO 'x' // Deve ser uma letra não presente nos nomes das funções.
#define NUMEROPARTICOESSOMARIEMANN 100 // Para Cálculo Integral.
#define VERDADE 1
#define FALSIDADE 0
#define INTERVALOPROGRESSO 100 // Para não haver flood quando mostrando progressos de processos.
#define INTERVALOPROGRESSO2 500000 // Para não haver flood quando mostrando progressos de processos, para processos mais rápidos.
#define APROXIMACAO 0.0000000001L // Para verificação de aproximação numérica.
#define APROXIMACAO2 0.0001L // Para verificação de aproximação numérica. Segunda opção.

typedef struct {char token [TAMANHO_BUFFER_WORD]; long double valor; char comentario [TAMANHO_BUFFER_PHRASE];} tokenfuncaoconstante; // Estrutura para funções e constantes.

typedef struct {long double real; long double img;} numerocomplexo; // Estrutura número complexo.

#define ARQUIVO_MATHSOBRE "/usr/share/antoniovandre_sobre.txt"
// #define ARQUIVO_MATH_ESTATISTICAS "antoniovandre_math_estatisticas.txt"
#define ARQUIVO_PRECISAO_REAL "/usr/share/antoniovandre_precisao_real.txt"

// Função de output do sobre/about dos softwares matemáticos.

int antoniovandre_mathsobre ()
	{
	FILE * filesobre;
	char antoniovandre_sobre_buffer_char;

	filesobre = fopen (ARQUIVO_MATHSOBRE, "r");

	if (filesobre == NULL)
		{
		printf ("Erro ao abrir arquivo sobre.\n");
		return -1;
		}

	while (! feof (filesobre))
		{
		antoniovandre_sobre_buffer_char = fgetc (filesobre);
		if (! feof(filesobre)) printf ("%c", antoniovandre_sobre_buffer_char);
		}

	fclose (filesobre);
	}

// Função de salvar as estatísticas de uso dos softwares matemáticos.

int antoniovandre_salvarmathestatisticas (char * cabecalho)
	{
	FILE * filemathestatisticas;
	char antoniovandre_estatisticas_buffer [TAMANHO_BUFFER_WORD];
	unsigned long int antoniovandre_estatisticas_contador;
	int flag = 0;
	int flag2 = 0;
	char tc;
	char tc2;

	strcpy (antoniovandre_estatisticas_buffer, "");

	filemathestatisticas = fopen (ARQUIVO_MATH_ESTATISTICAS, "r+");

	if (filemathestatisticas == NULL)
		{
		filemathestatisticas = fopen (ARQUIVO_MATH_ESTATISTICAS, "w");

		if (filemathestatisticas == NULL)
			{
			printf ("Erro ao abrir ou criar arquivo de estatísticas.\n");
			return -1;
			}

		flag2 = 1;
		}

	while ((! feof (filemathestatisticas)) && flag2 == 0)
		{
		fscanf (filemathestatisticas, "%s", antoniovandre_estatisticas_buffer);

		if (! strcmp (antoniovandre_estatisticas_buffer, cabecalho))
			{
			fscanf (filemathestatisticas, "%s", antoniovandre_estatisticas_buffer);

			antoniovandre_estatisticas_contador = atoi (antoniovandre_estatisticas_buffer);
			antoniovandre_estatisticas_contador++;

			fseek (filemathestatisticas, (-1) * strlen (antoniovandre_estatisticas_buffer), SEEK_CUR);

			fprintf (filemathestatisticas, "%lu", antoniovandre_estatisticas_contador);

			if ((int) log10 (antoniovandre_estatisticas_contador - 1) != (int) log10(antoniovandre_estatisticas_contador))
				{
				tc = fgetc (filemathestatisticas);
				if (! feof (filemathestatisticas)) fseek (filemathestatisticas, (-1) * sizeof (char), SEEK_CUR);
				fputc ('\n', filemathestatisticas);

				if (! feof (filemathestatisticas))
					do
						{
						tc2 = fgetc (filemathestatisticas);
						if (! feof (filemathestatisticas)) fseek (filemathestatisticas, (-1) * sizeof (char), SEEK_CUR);
						fputc (tc, filemathestatisticas);
						tc = tc2;
						} while (! feof (filemathestatisticas));
				}

			flag = 1;
			}
		}

	if (flag == 0)
		{
		char cabecalho_buffer [strlen (cabecalho)];

		strcpy (cabecalho_buffer, cabecalho);
		strcat (cabecalho_buffer, " 1\n");

		fprintf (filemathestatisticas, "%s", cabecalho_buffer);
		}

	fclose (filemathestatisticas);
	}

// Função que retorna a precisão real (numérica) para floats.

int antoniovandre_precisao_real ()
	{
	FILE * fileprecisaoreal;
	char antoniovandre_precisao_real_buffer [TAMANHO_BUFFER_WORD];
	int antoniovandre_precisao_real_valor;

	strcpy (antoniovandre_precisao_real_buffer, "");

	fileprecisaoreal = fopen(ARQUIVO_PRECISAO_REAL, "r");

	if (fileprecisaoreal == NULL)
		{
		printf ("Erro ao abrir arquivo de precisão real.\n");
		return -1;
		}

	fscanf (fileprecisaoreal, "%s", antoniovandre_precisao_real_buffer);

	antoniovandre_precisao_real_valor = atoi (antoniovandre_precisao_real_buffer);

	if (antoniovandre_precisao_real_valor == 0)
		{
		printf ("Erro ao ler arquivo de precisão real.\n");
		return -1;
		}

	fclose (fileprecisaoreal);

	return antoniovandre_precisao_real_valor;
	}

// Array de letras.

const char * antoniovandre_letras = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Array de letras maiúsculas.

const char * antoniovandre_letrasmaiusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Array de letras minúsculas.

const char * antoniovandre_letrasminusculas = "abcdefghijklmnopqrstuvwxyz";

// Array de números.

const char * antoniovandre_numeros = ".-0123456789";

// Array de operadores.

const char * antoniovandre_operadores = "+-*/^%@#$><:~";

// Array de operadores especiais.

const char * antoniovandre_operadoresespeciais = "%@#$><:~";

// Array de operadores prioritários.

const char * antoniovandre_operadoresprioritarios = "^";

// Remover letras de uma string.

char * antoniovandre_removerletras (char * str)
	{
	int tam1 = strlen (str);
	int tam2 = strlen (antoniovandre_letras);
	char strf [TAMANHO_BUFFER_PHRASE];
	int i;
	int j;
	int flag;
	int contador = 0;

	strcpy (strf, "");

	for (i = 0; i < tam1; i++)
		{
		flag = 0;

		for (j = 0; j < tam2; j++)
			if (str [i] == antoniovandre_letras [j]) flag = 1;

		if (flag == 0) strf [contador++] = str [i];
		}

	return strf;
	}

// Remover números de uma string.

char * antoniovandre_removernumeros (char * str)
	{
	int tam1 = strlen (str);
	int tam2 = strlen (antoniovandre_numeros);
	char strf [TAMANHO_BUFFER_PHRASE];
	int i;
	int j;
	int flag;

	strcpy (strf, "");

	for (i = 0; i < tam1; i++)
		{
		flag = 0;

		for (j = 0; j < tam2; j++)
			if (str [i] == antoniovandre_numeros [j]) flag = 1;

		if (flag == 0) strncat (strf, & str [i], 1);
		}

	return strf;
	}

// Verificar se string é monômio.

int antoniovandre_monomio (char * str)
	{
	int contador = 0;
	int flag;
	int i;
	int j;

	for (i = 1; i < strlen (str); i++)
		{
		flag = 0;

		for (j = 0; j < strlen (antoniovandre_numeros); j++)
			if (str [i] == antoniovandre_numeros [j]) flag = 1;

		if (flag == 1)
			for (j = 0; j < strlen (antoniovandre_letras); j++)
				if (str [i - 1] == antoniovandre_letras [j]) return 0;
		}

	for (i = 1; i < strlen (str); i++) if (str [i] == '-') return 0;

	for (i = 0; i < strlen (str); i++)
		{if (str [i] == '.') contador++; if (contador == 2) return 0;}

	if (strcmp (antoniovandre_removerletras (antoniovandre_removernumeros (str)), "") || (! strcmp (str, "-")))
		return 0;
	else
		return -1;
	}

// Parte numérica de um monômio.

long double antoniovandre_partenumericamonomio (char * str)
	{
	char * err;

	if (antoniovandre_monomio (str))
		{
		if (! strcmp (antoniovandre_removerletras (str), ""))
			return 1;
		else if (! strcmp (antoniovandre_removerletras (str), "-") && strlen (str) != 1)
			return -1;
		else
			return (strtold (antoniovandre_removerletras (str), & err));
		}
	else
		return 0;
	}

// Parte literal de um monômio.

char * antoniovandre_parteliteralmonomio (char * str)
	{
	if (antoniovandre_monomio (str))
		return antoniovandre_removernumeros (str);
	else
		return NULL;
	}

// Número para string.

char * antoniovandre_numeroparastring (long double numero)
	{
	int precisao = antoniovandre_precisao_real ();
	int potencia_min = (-1) * precisao;
	int potencia_max = log10 (VALOR_MAX);
	long double fator = powl (10, potencia_max);
	char strr [TAMANHO_BUFFER_WORD];
	int algarismo;
	int i;
	int flag = 0;
	int contador = 0;

	strcpy (strr, "");

	if (numero < 0)
		{
		numero *= -1;
		strcat (strr, "-");
		}

	if (numero > VALOR_MAX) return STRINGSAIDAERROOVER;

	for (i = potencia_max; i >= potencia_min; i--)
		{
		if (i == -1)
			{
			if (numero < 1) strcat (strr, "0");

			strcat (strr, ".");
			}

		algarismo = (int) ((int) ((long double) numero / (long double) fator) % 10);

		fator /= (long double) 10;

		if (algarismo != 0) flag = 1;

		if (flag == 1) contador++;

		if ((flag == 1 || i < 0) && (contador <= precisao))
			{
			switch (algarismo)
				{
				case 0:
					strcat (strr, "0");
					break;
				case 1:
					strcat (strr, "1");
					break;
				case 2:
					strcat (strr, "2");
					break;
				case 3:
					strcat (strr, "3");
					break;
				case 4:
					strcat (strr, "4");
					break;
				case 5:
					strcat (strr, "5");
					break;
				case 6:
					strcat (strr, "6");
					break;
				case 7:
					strcat (strr, "7");
					break;
				case 8:
					strcat (strr, "8");
					break;
				case 9:
					strcat (strr, "9");
					break;
				default:
					strcat (strr, "0");
				}
			}
		}

	return strr;
	}

// Comparar strings sem ordem.

int antoniovandre_compararstringssemorden (char * str1, char * str2)
	{
	int tam1 = strlen (str1);
	int tam2 = strlen (str2);
	char c;
	int contador1;
	int contador2;
	int i;
	int j;

	for (i = 0; i < tam1; i++)
		{
		c = str1 [i];

		contador1 = 0;

		for (j = 0; j < tam1; j++) if (c == str1 [j]) contador1++;

		contador2 = 0;

		for (j = 0; j < tam2; j++) if (c == str2 [j]) contador2++;

		if (contador1 != contador2) return -1;
		}

	if (tam1 != tam2) return -1;

	return 0;
	}

// Enésima substring de uma string separada por delimitadores. Inicia-se contando do 0;

char * antoniovandre_nthsubstr (char * str, int n)
	{
	char * strf = (char *) malloc (TAMANHO_BUFFER_PHRASE);
	int tam = strlen (str);
	int inicio = 0;
	int fim = 0;
	int ponteiro = 0;
	int shifti = 0;
	int shiftf = 0;
	int contador = 0;
	int i;

	strcpy (strf, "");

	for (i = 0; i < tam; i++)
		if (str [i] == DELIMITADORSTRING)
			if (n == contador++)
				{
				inicio = ponteiro;
				fim = i;
				}
			else
				ponteiro = i;

	if (fim == 0)
		{
		inicio = ponteiro;
		fim = tam - 1;
		}

	if (inicio != 0) shifti = 1;
	if (fim != tam - 1) shiftf = 1;

	contador = 0;

	for (i = inicio + shifti; i <= fim - shiftf; i++) strf [contador++] = str [i];

	return strf;
	}

// Fatorial.

unsigned long int antoniovandre_fatorial (unsigned long int n)
	{
	if (n > 1) return antoniovandre_fatorial (n - 1) * (unsigned long int) n; else return 1;
	}

// Reduzir termos semelhantes.

char * antoniovandre_reduzirtermossemelhantes (char * args)
	{
	int nargs = 1;
	char strf [TAMANHO_BUFFER_WORD];
	char parteliteral [TAMANHO_BUFFER_WORD];
	char strt [TAMANHO_BUFFER_WORD];
	int i;
	int j;
	int flag;
	int contador = 0;

	for (i = 0; i < strlen (args); i++) if (args [i] == DELIMITADORSTRING) nargs++;

	char * strlit [nargs];
	long double coefs [nargs];

	strcpy (strf, "");
	strcpy (parteliteral, "");
	strcpy (strt, "");

	for (i = 0; i < nargs; i++)
		{
		strlit [i] = (char *) malloc (TAMANHO_BUFFER_WORD);
		strcpy (strlit [i], "");
		}

	for (i = 0; i < nargs; i++)
		{
		flag = 0; j = 0;

		while (flag == 0 && j++ < TENTATIVASLOGICAS)
			if (antoniovandre_monomio (antoniovandre_nthsubstr (args, i))) flag = 1;

		if (flag == 1)
			{
			strcpy (parteliteral, antoniovandre_parteliteralmonomio (antoniovandre_nthsubstr (args, i)));

			flag = 0;

			for (j = 0; j < contador; j++)
				if (! antoniovandre_compararstringssemorden (strlit [j], parteliteral))
					{
					coefs [j] += (long double) antoniovandre_partenumericamonomio (antoniovandre_nthsubstr (args, i));
					flag = 1;
					break;
					}
			if (flag == 0)
				{
				strcpy (strlit [contador], parteliteral);
				coefs [contador++] = (long double) antoniovandre_partenumericamonomio (antoniovandre_nthsubstr (args, i));
				}
			}
		else
			return (STRINGSAIDAERRO);
		}

	flag = 0;

	for (i = 0; i < contador; i++)
		if (coefs [i] != 0)
			{
			strcpy (strt, antoniovandre_numeroparastring (coefs [i]));

			if (! strcmp (strt, STRINGSAIDAERROOVER)) return (STRINGSAIDAERROOVER);

			if (i == 0) 
				memmove (strf, strt, strlen (antoniovandre_numeroparastring (coefs [i])));
			else
				strcat (strf, strt);
			strcat (strf, strlit [i]);
			if (i < contador - 1) strf [strlen (strf)] = ' ';

			flag = 1;
			}

	for (i = 0; i < nargs; i++) free (strlit [i]);

	if (flag == 1)
		return (strf);
	else
		return (antoniovandre_numeroparastring (0));
	}

// Valor numérico de um polinômio.

char * antoniovandre_valornumericopolinomio (char * args)
	{
	int nargs = 1;
	int indice_inicio = -1;
	char strt [TAMANHO_BUFFER_WORD];
	char strt2 [TAMANHO_BUFFER_WORD];
	char str [TAMANHO_BUFFER_WORD];
	char str2 [TAMANHO_BUFFER_WORD];
	char str3 [TAMANHO_BUFFER_WORD];
	char str4 [TAMANHO_BUFFER_PHRASE];
	char str5 [TAMANHO_BUFFER_PHRASE];
	long double coef;
	long double fator;
	int i;
	int j;
	int k;
	int contador;
	int flag;
	int flag2 = 0;
	char c = DELIMITADORSTRING;
	char * err;

	strcpy (strt, "");
	strcpy (strt2, "");
	strcpy (str, "");
	strcpy (str2, "");
	strcpy (str3, "");
	strcpy (str4, "");
	strcpy (str5, "");

	for (i = 0; i < strlen (args); i++)
		{
		if (args [i] == DELIMITADORSTRING) nargs++;
		if ((args [i] == '=') && (indice_inicio == -1)) indice_inicio = nargs - 1;
		}

	if (indice_inicio != -1)
		{
		if (indice_inicio == 0) return STRINGSAIDAERRO;

		for (i = indice_inicio; i < nargs; i++)
			{
			flag = 0; contador = 0;

			while ((flag == 0) && (contador++ < TENTATIVASLOGICAS))
				{
				strcpy (strt, antoniovandre_nthsubstr (args, i));

				if (strt [1] == '=') flag = 1;
				}

			if (flag == 0) return STRINGSAIDAERRO;

			flag = 0;

			for (j = 0; j < strlen (antoniovandre_letras); j++)
				if (strt [0] == antoniovandre_letras [j]) flag = 1;

			if (flag == 0) return STRINGSAIDAERRO;

			strcpy (strt2, "");

			for (j = 2; j < strlen (strt); j++) strncat (strt2, & strt [j], 1);

			fator = strtold (strt2, & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (fator > VALOR_MAX) return STRINGSAIDAERROOVER;

			for (j = 0; j < indice_inicio; j++)
				{
				flag = 0; contador = 0;

				while ((flag == 0) && (contador++ < TENTATIVASLOGICAS))
					{
					if (flag2 == 0)
						strcpy (str, antoniovandre_nthsubstr (args, j));
					else
						strcpy (str, antoniovandre_nthsubstr (str5, j));

					if (antoniovandre_monomio (str)) flag = 1;
					}

				if (flag == 0) return STRINGSAIDAERRO;

				coef = antoniovandre_partenumericamonomio (str);

				if (coef > VALOR_MAX) return STRINGSAIDAERROOVER;

				strcpy (str2, antoniovandre_parteliteralmonomio (str));

				strcpy (str3, "");

				for (k = 0; k < strlen (str2); k++)
					if (strt [0] == str2 [k])
						coef *= fator;
					else
						strncat (str3, & str2 [k], 1);

				if (coef > VALOR_MAX) return STRINGSAIDAERROOVER;

				strcat (str4, antoniovandre_numeroparastring (coef));
				strcat (str4, str3);
				strncat (str4, &c, 1);
				}

			str4 [strlen (str4) - 1] = '\0';
			strcpy (str5, str4);
			strcpy (str4, "");

			flag2 = 1;
			}
		}

	if (indice_inicio != -1)
		return antoniovandre_reduzirtermossemelhantes (str5);
	else
		return antoniovandre_reduzirtermossemelhantes (args);
	}

// Função de verificação de uma expressão algébrica.

int antoniovandre_expressao (char * str)
	{
	int flag;
	int i;
	int j;

	for (i = 0; i < strlen (str); i++)
		{
		flag = 0;

		for (j = 0; j < strlen (antoniovandre_numeros); j++) if (str [i] == antoniovandre_numeros [j]) flag = 1;

		for (j = 0; j < strlen (antoniovandre_letras); j++) if (str [i] == antoniovandre_letras [j]) flag = 1;

		for (j = 0; j < strlen (antoniovandre_operadores); j++) if (str [i] == antoniovandre_operadores [j]) flag = 1;

		if ((str [i] == TOKENINICIOEVAL) || (str [i] == TOKENFIMEVAL)) flag = 1;

		if (flag == 0) return 0;
		}

	return 1;
	}

// Substring.

char * antoniovandre_substring (char * str, int inicio, int fim)
	{
	char strt [TAMANHO_BUFFER_PHRASE];
	int i;

	strcpy (strt, "");

	for (i = inicio; i <= fim; i++)
		strncat (strt, & str [i], 1);

	return strt;
	}

// Função eval célula função. Os nomes das funções devem conter apenas letras minúsculas. Os nomes das constantes devem conter apenas letras maiúsculas. No escopo, as implementações das funções devem estar dispostas de modo a que os nomes das anteriores não sejam substrings das posteriores.

char * antoniovandre_evalcelulafuncao (char * str)
	{
	tokenfuncaoconstante funcoesconstantes [TAMANHO_BUFFER_SMALL];
	int tamanhotokenfuncaoconstante;
	int tamanhotokenfuncaoconstantemax = 0;
	int tokenid = -1;
	int tokeninicio;
	long double resultado;
	long double argumento;
	long double coeficiente;
	char buffer [TAMANHO_BUFFER_WORD];
	int i;
	int j;
	char * err;
	char * err2;

	for (i = 0; i < TAMANHO_BUFFER_SMALL; i++)
		{
		strcpy (funcoesconstantes [i].token, "");
		funcoesconstantes [i].valor = (long double) 0;
		strcpy (funcoesconstantes [i].comentario, "");
		}

	strcpy (funcoesconstantes [0].token, "arccossec");
	strcpy (funcoesconstantes [0].comentario, "Função arco-cossecante.");

	strcpy (funcoesconstantes [1].token, "arcsec");
	strcpy (funcoesconstantes [1].comentario, "Função arco-secante.");

	strcpy (funcoesconstantes [2].token, "arccotg");
	strcpy (funcoesconstantes [2].comentario, "Função arco-cotangente.");

	strcpy (funcoesconstantes [3].token, "arctg");
	strcpy (funcoesconstantes [3].comentario, "Função arco-tangente.");

	strcpy (funcoesconstantes [4].token, "arccos");
	strcpy (funcoesconstantes [4].comentario, "Função arco-cosseno.");

	strcpy (funcoesconstantes [5].token, "arcsen");
	strcpy (funcoesconstantes [5].comentario, "Função arco-seno.");

	strcpy (funcoesconstantes [6].token, "cossec");
	strcpy (funcoesconstantes [6].comentario, "Função cossecante.");

	strcpy (funcoesconstantes [7].token, "sec");
	strcpy (funcoesconstantes [7].comentario, "Função secante.");

	strcpy (funcoesconstantes [8].token, "sen");
	strcpy (funcoesconstantes [8].comentario, "Função seno.");

	strcpy (funcoesconstantes [9].token, "cos");
	strcpy (funcoesconstantes [9].comentario, "Função cosseno.");

	strcpy (funcoesconstantes [10].token, "cotg");
	strcpy (funcoesconstantes [10].comentario, "Função cotangente.");

	strcpy (funcoesconstantes [11].token, "tg");
	strcpy (funcoesconstantes [11].comentario, "Função tangente.");

	strcpy (funcoesconstantes [12].token, "logdois");
	strcpy (funcoesconstantes [12].comentario, "Função logaritmo na base 2.");

	strcpy (funcoesconstantes [13].token, "logdez");
	strcpy (funcoesconstantes [13].comentario, "Função logaritmo na base 10.");

	strcpy (funcoesconstantes [14].token, "ln");
	strcpy (funcoesconstantes [14].comentario, "Função logaritmo natural.");

	strcpy (funcoesconstantes [15].token, "PI");
	funcoesconstantes [15].valor = (long double) M_PIl;
	strcpy (funcoesconstantes [15].comentario, "Constante pi, razão entre o comprimento e o diâmetro de uma circunferência.");

	strcpy (funcoesconstantes [16].token, "E");
	funcoesconstantes [16].valor = (long double) M_El;
	strcpy (funcoesconstantes [16].comentario, "Constante e, base do logaritmo natural.");

	strcpy (funcoesconstantes [17].token, FISICA_C_SI);
	funcoesconstantes [17].valor = (long double) FISICA_C_SI_VALOR;
	strcpy (funcoesconstantes [17].comentario, FISICA_C_SI_COMENTARIO);

	strcpy (funcoesconstantes [18].token, FISICA_G_SI);
	funcoesconstantes [18].valor = (long double) FISICA_G_SI_VALOR;
	strcpy (funcoesconstantes [18].comentario, FISICA_G_SI_COMENTARIO);

	strcpy (funcoesconstantes [19].token, FISICA_QE_SI);
	funcoesconstantes [19].valor = (long double) FISICA_QE_SI_VALOR;
	strcpy (funcoesconstantes [19].comentario, FISICA_QE_SI_COMENTARIO);

	strcpy (funcoesconstantes [20].token, FISICA_H_SI);
	funcoesconstantes [20].valor = (long double) FISICA_H_SI_VALOR;
	strcpy (funcoesconstantes [20].comentario, FISICA_H_SI_COMENTARIO);

	strcpy (funcoesconstantes [21].token, FISICA_HH_SI);
	funcoesconstantes [21].valor = (long double) FISICA_HH_SI_VALOR;
	strcpy (funcoesconstantes [21].comentario, FISICA_HH_SI_COMENTARIO);

	strcpy (funcoesconstantes [22].token, FISICA_K_SI);
	funcoesconstantes [22].valor = (long double) FISICA_K_SI_VALOR;
	strcpy (funcoesconstantes [22].comentario, FISICA_K_SI_COMENTARIO);

	strcpy (funcoesconstantes [23].token, "senh");
	strcpy (funcoesconstantes [23].comentario, "Função seno hiperbólico.");

	strcpy (funcoesconstantes [24].token, "cosh");
	strcpy (funcoesconstantes [24].comentario, "Função cosseno hiperbólico.");

	strcpy (funcoesconstantes [25].token, "tgh");
	strcpy (funcoesconstantes [25].comentario, "Função tangente hiperbólica.");

	strcpy (funcoesconstantes [26].token, "cotgh");
	strcpy (funcoesconstantes [26].comentario, "Função cotangente hiperbólica.");

	strcpy (funcoesconstantes [27].token, "sech");
	strcpy (funcoesconstantes [27].comentario, "Função secante hiperbólica.");

	strcpy (funcoesconstantes [28].token, "cossech");
	strcpy (funcoesconstantes [28].comentario, "Função cossecante hiperbólica.");

	strcpy (funcoesconstantes [29].token, "arcsenh");
	strcpy (funcoesconstantes [29].comentario, "Função arco-seno hiperbólico.");

	strcpy (funcoesconstantes [30].token, "arccosh");
	strcpy (funcoesconstantes [30].comentario, "Função arco-cosseno hiperbólico.");

	strcpy (funcoesconstantes [31].token, "arctgh");
	strcpy (funcoesconstantes [31].comentario, "Função arco-tangente hiperbólico.");

	strcpy (funcoesconstantes [32].token, "arccotgh");
	strcpy (funcoesconstantes [32].comentario, "Função arco-cotangente hiperbólico.");

	strcpy (funcoesconstantes [33].token, "arcsech");
	strcpy (funcoesconstantes [33].comentario, "Função arco-secante hiperbólico.");

	strcpy (funcoesconstantes [34].token, "arccossech");
	strcpy (funcoesconstantes [34].comentario, "Função arco-cossecante hiperbólico.");

	strcpy (funcoesconstantes [35].token, "fatorial");
	strcpy (funcoesconstantes [35].comentario, "Função fatorial.");

	strcpy (funcoesconstantes [36].token, FISICA_ME_SI);
	funcoesconstantes [36].valor = (long double) FISICA_ME_SI_VALOR;
	strcpy (funcoesconstantes [36].comentario, FISICA_ME_SI_COMENTARIO);

	strcpy (funcoesconstantes [37].token, FISICA_MP_SI);
	funcoesconstantes [37].valor = (long double) FISICA_MP_SI_VALOR;
	strcpy (funcoesconstantes [37].comentario, FISICA_MP_SI_COMENTARIO);

//	Trecho incompatível com alguns compiladores.

/*	strcpy (funcoesconstantes [38].token, FISICA_A_SI);
	funcoesconstantes [38].valor = (long double) FISICA_A_SI_VALOR;
	strcpy (funcoesconstantes [38].comentario, FISICA_A_SI_COMENTARIO);*/

	strcpy (funcoesconstantes [39].token, "modulo");
	strcpy (funcoesconstantes [39].comentario, "Função módulo.");

	strcpy (funcoesconstantes [40].token, "radparagrau");
	strcpy (funcoesconstantes [40].comentario, "Função para converter radianos para graus.");

	strcpy (funcoesconstantes [41].token, "radparagrado");
	strcpy (funcoesconstantes [41].comentario, "Função para converter radianos para grados.");

	strcpy (funcoesconstantes [42].token, "graupararad");
	strcpy (funcoesconstantes [42].comentario, "Função para converter graus para radianos.");

	strcpy (funcoesconstantes [43].token, "grauparagrado");
	strcpy (funcoesconstantes [43].comentario, "Função para converter graus para grados.");

	strcpy (funcoesconstantes [44].token, "gradopararad");
	strcpy (funcoesconstantes [44].comentario, "Função para converter grados para radianos.");

	strcpy (funcoesconstantes [45].token, "gradopargrau");
	strcpy (funcoesconstantes [45].comentario, "Função para converter grados para graus.");

	strcpy (funcoesconstantes [46].token, "kelvinparacelsius");
	strcpy (funcoesconstantes [46].comentario, "Função para converter temperatura em Kelvin para Celsius.");

	strcpy (funcoesconstantes [47].token, "kelvinparafahrenheit");
	strcpy (funcoesconstantes [47].comentario, "Função para converter temperatura em Kelvin para Fahrenheit.");

	strcpy (funcoesconstantes [48].token, "celsiusparakelvin");
	strcpy (funcoesconstantes [48].comentario, "Função para converter temperatura em Celsius para Kelvin.");

	strcpy (funcoesconstantes [49].token, "celsiusparafahrenheit");
	strcpy (funcoesconstantes [49].comentario, "Função para converter temperatura em Celsius para Fahrenheit.");

	strcpy (funcoesconstantes [50].token, "fahrenheitparakelvin");
	strcpy (funcoesconstantes [50].comentario, "Função para converter temperatura em Fahrenheit para Kelvin.");

	strcpy (funcoesconstantes [51].token, "fahrenheitparacelsius");
	strcpy (funcoesconstantes [51].comentario, "Função para converter temperatura em Fahrenheit para Celsius.");

	strcpy (funcoesconstantes [52].token, FISICA_AM_SI);
	funcoesconstantes [52].valor = (long double) FISICA_AM_SI_VALOR;
	strcpy (funcoesconstantes [52].comentario, FISICA_AM_SI_COMENTARIO);

	strcpy (funcoesconstantes [53].token, FISICA_KE_SI);
	funcoesconstantes [53].valor = (long double) FISICA_KE_SI_VALOR;
	strcpy (funcoesconstantes [53].comentario, FISICA_KE_SI_COMENTARIO);

	strcpy (funcoesconstantes [54].token, MATEMATICA_P);
	funcoesconstantes [54].valor = (long double) MATEMATICA_P_VALOR;
	strcpy (funcoesconstantes [54].comentario, MATEMATICA_P_COMENTARIO);

	strcpy (funcoesconstantes [55].token, MATEMATICA_C);
	funcoesconstantes [55].valor = (long double) MATEMATICA_C_VALOR;
	strcpy (funcoesconstantes [55].comentario, MATEMATICA_C_COMENTARIO);

	strcpy (funcoesconstantes [56].token, MATEMATICA_EMC);
	funcoesconstantes [56].valor = (long double) MATEMATICA_EMC_VALOR;
	strcpy (funcoesconstantes [56].comentario, MATEMATICA_EMC_COMENTARIO);

	strcpy (funcoesconstantes [57].token, MATEMATICA_T);
	funcoesconstantes [57].valor = (long double) MATEMATICA_T_VALOR;
	strcpy (funcoesconstantes [57].comentario, MATEMATICA_T_COMENTARIO);

	strcpy (funcoesconstantes [58].token, FISICA_R_SI);
	funcoesconstantes [58].valor = (long double) FISICA_R_SI_VALOR;
	strcpy (funcoesconstantes [58].comentario, FISICA_R_SI_COMENTARIO);

	strcpy (funcoesconstantes [59].token, "piso");
	strcpy (funcoesconstantes [59].comentario, "Função piso.");

	strcpy (funcoesconstantes [60].token, "teto");
	strcpy (funcoesconstantes [60].comentario, "Função teto.");

	strcpy (funcoesconstantes [61].token, FISICA_SB_SI);
	funcoesconstantes [61].valor = (long double) FISICA_SB_SI_VALOR;
	strcpy (funcoesconstantes [61].comentario, FISICA_SB_SI_COMENTARIO);

	strcpy (funcoesconstantes [62].token, FISICA_PM_SI);
	funcoesconstantes [62].valor = (long double) FISICA_PM_SI_VALOR;
	strcpy (funcoesconstantes [62].comentario, FISICA_PM_SI_COMENTARIO);

	strcpy (funcoesconstantes [63].token, FISICA_PE_SI);
	funcoesconstantes [63].valor = (long double) FISICA_PE_SI_VALOR;
	strcpy (funcoesconstantes [63].comentario, FISICA_PE_SI_COMENTARIO);

	strcpy (funcoesconstantes [64].token, FISICA_B_SI);
	funcoesconstantes [64].valor = (long double) FISICA_B_SI_VALOR;
	strcpy (funcoesconstantes [64].comentario, FISICA_B_SI_COMENTARIO);

	strcpy (funcoesconstantes [65].token, FISICA_MN_SI);
	funcoesconstantes [65].valor = (long double) FISICA_MN_SI_VALOR;
	strcpy (funcoesconstantes [65].comentario, FISICA_MN_SI_COMENTARIO);

	strcpy (funcoesconstantes [66].token, MATEMATICA_FA);
	funcoesconstantes [66].valor = (long double) MATEMATICA_FA_VALOR;
	strcpy (funcoesconstantes [66].comentario, MATEMATICA_FA_COMENTARIO);

	strcpy (funcoesconstantes [67].token, MATEMATICA_FS);
	funcoesconstantes [67].valor = (long double) MATEMATICA_FS_VALOR;
	strcpy (funcoesconstantes [67].comentario, MATEMATICA_FS_COMENTARIO);

	strcpy (funcoesconstantes [68].token, MATEMATICA_CO);
	funcoesconstantes [68].valor = (long double) MATEMATICA_CO_VALOR;
	strcpy (funcoesconstantes [68].comentario, MATEMATICA_CO_COMENTARIO);

	strcpy (funcoesconstantes [69].token, MATEMATICA_K);
	funcoesconstantes [69].valor = (long double) MATEMATICA_K_VALOR;
	strcpy (funcoesconstantes [69].comentario, MATEMATICA_K_COMENTARIO);

	strcpy (funcoesconstantes [70].token, MATEMATICA_GK);
	funcoesconstantes [70].valor = (long double) MATEMATICA_GK_VALOR;
	strcpy (funcoesconstantes [70].comentario, MATEMATICA_GK_COMENTARIO);

	for (i = 0; i < strlen (str); i++)
		for (j = 0; j < TAMANHO_BUFFER_SMALL; j++)
			if (! strcmp (antoniovandre_substring (str, i, i + strlen (funcoesconstantes [j].token) - 1), funcoesconstantes [j].token))
				{
				tamanhotokenfuncaoconstante = strlen (funcoesconstantes [j].token);

				if (tamanhotokenfuncaoconstantemax < tamanhotokenfuncaoconstante)
					{
					tamanhotokenfuncaoconstantemax = tamanhotokenfuncaoconstante;
					tokenid = j;
					tokeninicio = i;
					}
				}

	if (tokenid != -1)
		if (funcoesconstantes [tokenid].valor != 0)
			{
			coeficiente = 1;

			if (tokeninicio > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, tokeninicio - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}	
				}

			if (strcmp (antoniovandre_substring (str, tokeninicio + tamanhotokenfuncaoconstantemax, strlen (str) - 1), "")) return STRINGSAIDAERRO;

			return antoniovandre_numeroparastring ((long double) coeficiente * funcoesconstantes [tokenid].valor);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 3), "teto"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 4, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			if (argumento <= 0)
				resultado = (long int) argumento;
			else
				{
					if (argumento == (long int) argumento)
						resultado = (long int) argumento;
					else
						resultado = (long int) argumento + 1;
				}

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) resultado);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 3), "piso"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 4, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			if (argumento >= 0)
				resultado = (long int) argumento;
			else
				{
					if (argumento == (long int) argumento)
						resultado = (long int) argumento;
					else
						resultado = (long int) argumento - 1;
				}

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) resultado);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 20), "fahrenheitparacelsius"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 21, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) (5 * (argumento - 32) / 9));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 19), "fahrenheitparakelvin"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 20, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) ((5 * (argumento - 32) / 9) + 273.15));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 20), "celsiusparafahrenheit"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 21, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) ((9 * argumento / 5) + 32));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 16), "celsiusparakelvin"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 17, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) (argumento - 273.15));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 19), "kelvinparafahrenheit"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 20, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) ((9 * (argumento - 273.15) / 5) + 32));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 16), "kelvinparacelsius"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 17, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) (argumento + 273.15));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 12), "gradoparagrau"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 13, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) argumento * 9 / 10);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 11), "gradopararad"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 12, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) argumento * M_PIl / 200);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 12), "grauparagrado"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 13, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) argumento * 10 / 9);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 10), "graupararad"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 11, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) argumento * M_PIl / 180);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 11), "radparagrado"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 12, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) argumento * 200 / M_PIl);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 10), "radparagrau"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 11, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) argumento * 180 / M_PIl);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 5), "modulo"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 6, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) fabs (argumento));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 7), "fatorial"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 8, strlen (str) - 1), & err);

			if ((* err != 0) || ((argumento != (long int) argumento) || argumento < 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) coeficiente * (long double) antoniovandre_fatorial ((unsigned long int) argumento));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 9), "arccossech"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 10, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento == 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((long double) (logl ((long double) (1 + (long double) powl (1 + (long double) powl ((long double) argumento, 2), 0.5)) / ((long double) argumento))))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 6), "arcsech"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 7, strlen (str) - 1), & err);

			if ((* err != 0) || ((argumento <= 0) || (argumento > 1))) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((long double) (logl ((long double) (1 + (long double) powl (1 - (long double) powl ((long double) argumento, 2), 0.5)) / ((long double) argumento))))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 7), "arccotgh"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 8, strlen (str) - 1), & err);

			if ((* err != 0) || ((argumento >= -1) && (argumento <= 1))) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((long double) ((logl ((long double) ((long double) argumento + 1) / ((long double) argumento - 1))) / 2))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 5), "arctgh"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 6, strlen (str) - 1), & err);

			if ((* err != 0) || ((argumento <= -1) || (argumento >= 1))) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((long double) ((logl ((long double) (1 + (long double) argumento) / (1 - (long double) argumento))) / 2))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 6), "arccosh"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 7, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento < 1)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((long double) logl ((long double) (argumento + powl ((long double) (powl ((long double) argumento, 2) - 1), 0.5))))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 6), "arcsenh"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 7, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((long double) logl ((long double) (argumento + powl ((long double) (powl ((long double) argumento, 2) + 1), 0.5))))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 6), "cossech"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 7, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento == 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) (2 / (powl (M_El, (long double) argumento) - powl (M_El, ((-1) * (long double) argumento))))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 3), "sech"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 4, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) (2 / (powl (M_El, (long double) argumento) + powl (M_El, ((-1) * (long double) argumento))))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 4), "cotgh"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 5, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento == 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((powl (M_El, (long double) argumento) + powl (M_El, ((-1) * (long double) argumento))) / (powl (M_El, (long double) argumento) - powl (M_El, ((-1) * (long double) argumento))))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 2), "tgh"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 3, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((powl (M_El, (long double) argumento) - powl (M_El, ((-1) * (long double) argumento))) / (powl (M_El, (long double) argumento) + powl (M_El, ((-1) * (long double) argumento))))));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 3), "senh"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 4, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento == 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((powl (M_El, (long double) argumento) - powl (M_El, ((-1) * (long double) argumento))) / 2)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 3), "cosh"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 4, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento == 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * (long double) ((powl (M_El, (long double) argumento) + powl (M_El, ((-1) * (long double) argumento))) / 2)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 8), "arccossec"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 9, strlen (str) - 1), & err);

			if ((* err != 0) || ((argumento > -1) && (argumento < 1))) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * asinl (1 / (long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 5), "arcsec"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 6, strlen (str) - 1), & err);

			if ((* err != 0) || ((argumento > -1) && (argumento < 1))) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * acosl (1 / (long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 6), "arccotg"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 7, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			if (argumento == 0) resultado = (long double) coeficiente * M_PI_2l; else if (argumento < 0) resultado = (long double) coeficiente * (atanl (1 / (long double) argumento) + M_PIl); else resultado = (long double) coeficiente * atanl (1 / (long double) argumento);

			return antoniovandre_numeroparastring ((long double) resultado);
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 4), "arctg"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 5, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * atanl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 5), "arccos"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 6, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento < -1) || (argumento > 1)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * acosl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 5), "arcsen"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 6, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento < -1) || (argumento > 1)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * asinl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 5), "cossec"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 6, strlen (str) - 1), & err);

			if ((* err != 0) || (fmodl (argumento, M_PIl) == 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente / sinl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 2), "sec"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 3, strlen (str) - 1), & err);

			if ((* err != 0) || (fmodl (argumento, M_PIl) == strtold (antoniovandre_numeroparastring (M_PI_2l), & err2))) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente / cosl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 2), "sen"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 3, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * sinl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 2), "cos"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 3, strlen (str) - 1), & err);

			if (* err != 0) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * cosl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 3), "cotg"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 4, strlen (str) - 1), & err);

			if ((* err != 0) || (fmodl (argumento, M_PIl) == 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente / tanl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 1), "tg"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 2, strlen (str) - 1), & err);

			if ((* err != 0) || (fmodl (argumento, M_PIl) == strtold (antoniovandre_numeroparastring (M_PI_2l), & err2))) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * tanl ((long double) argumento)));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 6), "logdois"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 7, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento <= 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * logl ((long double) argumento) / M_LN2l));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 5), "logdez"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 6, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento <= 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * logl ((long double) argumento) / M_LN10l));
			}

	for (i = 0; i < strlen (str); i++)
		if (! strcmp (antoniovandre_substring (str, i, i + 1), "ln"))
			{
			coeficiente = 1;

			if (i > 0)
				{
				strcpy (buffer, antoniovandre_substring (str, 0, i - 1));

				if (! strcmp (buffer, "-"))
					coeficiente = -1;
				else
					{
					coeficiente = strtold (buffer, & err);
					if (* err != 0) return STRINGSAIDAERRO;
					if (coeficiente > VALOR_MAX) return STRINGSAIDAERROOVER;
					}
				}

			argumento = strtold (antoniovandre_substring (str, i + 2, strlen (str) - 1), & err);

			if ((* err != 0) || (argumento <= 0)) return STRINGSAIDAERRO;

			if (argumento > VALOR_MAX) return STRINGSAIDAERROOVER;

			return antoniovandre_numeroparastring ((long double) ((long double) coeficiente * logl ((long double) argumento)));
			}

	strtold (str, & err);

	if ((* err != 0) || (! strcmp (str, "")))
		return STRINGSAIDAERRO;
	else
		return str;
	}

// Função eval célula.

char * antoniovandre_evalcelula (char * str)
	{
	char strt [TAMANHO_BUFFER_PHRASE];
	char strt2 [TAMANHO_BUFFER_WORD];
	char strt3 [TAMANHO_BUFFER_WORD];
	char strtv1 [TAMANHO_BUFFER_WORD];
	char strtv2 [TAMANHO_BUFFER_WORD];
	char strt4 [TAMANHO_BUFFER_PHRASE];
	int posicoes_operadores [TAMANHO_BUFFER_PHRASE];
	long double valor;
	long double valort;
	long double valort2;
	int contador;
	int i;
	int j;
	int k;
	int flag;
	int flag2;
	int flag3;
	int flag4;
	int ponteiro;
	int ponteiroinicio;
	int ponteirofim;
	int finalponteirocelula;
	char * err;
	char tc = ' ';
	char tc2;

	strcpy (strt, "");

	for (i = 0; i < strlen (str); i++)
		if (str [i] != tc) strncat (strt, & str [i], 1);

	if (! strcmp (strt, "")) return STRINGSAIDAERRO;

	tc = strt [0];

	for (i = 0; i < strlen (antoniovandre_operadores); i++)
		{
		tc2 = antoniovandre_operadores [i];

		if ((tc == tc2) && (tc2 != '-'))
			return STRINGSAIDAERRO;
		}

	while (VERDADE)
		{
		for (i = 0; i < TAMANHO_BUFFER_PHRASE; i++) posicoes_operadores [i] = -1;

		contador = 0; flag4 = 0;

		for (i = 0; i < strlen (strt); i++)
			for (j = 0; j < strlen (antoniovandre_operadores); j++)
				{
				flag3 = 0;

				for (k = 0; k < strlen (antoniovandre_letrasminusculas); k++)
					{
					if (((i == 0) && (antoniovandre_operadores [j] == '-')) || ((contador > 0) && (posicoes_operadores [contador - 1] == i - 1) && (antoniovandre_operadores [j] == '-')) || ((i > 0) && (strt [i - 1] == antoniovandre_letrasminusculas [k]) && (antoniovandre_operadores [j] == '-')))
						{
						flag3 = 1;
						flag4 = 1;
						}
					}

				if ((strt [i] == antoniovandre_operadores [j]) && flag3 == 0) posicoes_operadores [contador++] = i;
				}

		if (posicoes_operadores [0] == -1) break;

		for (i = 0; i < TAMANHO_BUFFER_PHRASE; i++)
			if (posicoes_operadores [i] > 0)
				{
				flag = 0;
				flag2 = 0;

				for (j = 0; j < TAMANHO_BUFFER_PHRASE; j++)
					if (strt [posicoes_operadores [j]] == '^') flag2 = 1;

				for (j = 0; j < TAMANHO_BUFFER_PHRASE; j++)
					if (((strt [posicoes_operadores [j]] == OPERADORMULTIPLICACAO) || (strt [posicoes_operadores [j]] == '/'))) flag = 1;

				strcpy (strt2, "");

				if (i > 0)
					ponteiro = posicoes_operadores [i - 1] + 1;
				else
					ponteiro = 0;

				ponteiroinicio = ponteiro;

				for (j = ponteiro; j < posicoes_operadores [i]; j++)
					strncat (strt2, & strt [j], 1);

				strcpy (strt3, "");

				if (posicoes_operadores [i + 1] == -1)
					ponteiro = strlen (strt);
				else
					ponteiro = posicoes_operadores [i + 1];

				ponteirofim = ponteiro;

				for (j = posicoes_operadores [i] + 1; j < ponteiro; j++)
					strncat (strt3, & strt [j], 1);

				if ((! strcmp (strt2, "")) || (! strcmp (strt3, "")))
					return STRINGSAIDAERRO;

				strcpy (strtv1, antoniovandre_evalcelulafuncao (strt2));
				strcpy (strtv2, antoniovandre_evalcelulafuncao (strt3));

				if (! strcmp (strtv1, STRINGSAIDAERRO)) return STRINGSAIDAERRO;
				if (! strcmp (strtv2, STRINGSAIDAERRO)) return STRINGSAIDAERRO;

				if (! strcmp (strtv1, STRINGSAIDAERROOVER)) return STRINGSAIDAERROOVER;
				if (! strcmp (strtv2, STRINGSAIDAERROOVER)) return STRINGSAIDAERROOVER;

				valort = strtold (strtv1, & err);
				if (* err != 0) return STRINGSAIDAERRO;

				valort2 = strtold (strtv2, & err);
				if (* err != 0) return STRINGSAIDAERRO;

				if (strt [posicoes_operadores [i]] == '^')
					{
					if (fabsl ((long double) valort2) < 1)
						{
						if (valort < 0)
							{
							if ((fmodl ((1 / valort2), 2) > (-1) * APROXIMACAO2) && (fmodl ((1 / valort2), 2) < APROXIMACAO2))
								return STRINGSAIDAERRO;
							else if ((fmodl ((1 / valort2), 2) > 1 + (-1) * APROXIMACAO2) && (fmodl ((1 / valort2), 2) < 1 + APROXIMACAO2))
								valor = (-1) * powl (fabsl ((long double) valort), (long double) valort2);
							else
								valor = powl ((long double) valort, (long double) valort2);
							}
						else
							valor = powl ((long double) valort, (long double) valort2);
						}
					else
						valor = powl ((long double) valort, (long double) valort2);

					if (isnan (valor) || isinf (valor)) return STRINGSAIDAERRO;
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == OPERADORMULTIPLICACAO) && (flag2 == 0))
					{
					valor = (long double) valort * (long double) valort2;
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '/') && (flag2 == 0))
					{
					if (valort2 == 0) return STRINGSAIDAERRO;

					valor = (long double) valort / (long double) valort2;
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '+') && (flag == 0) && (flag2 == 0))
					{
					valor = (long double) valort + (long double) valort2;
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '-') && (flag == 0) && (flag2 == 0))
					{
					valor = (long double) valort - (long double) valort2;
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '%') && (flag == 0) && (flag2 == 0))
					{
					valor = fmodl ((long double) valort, (long double) valort2);
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '@') && (flag == 0) && (flag2 == 0))
					{
					if ((valort != (long int) valort) || (valort < 0) || (valort2 != (long int) valort2) || (valort2 < 0) || (valort < valort2)) return STRINGSAIDAERRO;
					valor = (long double) antoniovandre_fatorial ((unsigned long int) valort) / antoniovandre_fatorial ((unsigned long int) (valort - valort2));
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '#') && (flag == 0) && (flag2 == 0))
					{
					if ((valort != (long int) valort) || (valort < 0) || (valort2 != (long int) valort2) || (valort2 < 0) || (valort < valort2)) return STRINGSAIDAERRO;
					valor = (long double) antoniovandre_fatorial ((unsigned long int) valort) / (antoniovandre_fatorial ((unsigned long int) (valort - valort2)) * antoniovandre_fatorial ((unsigned long int) (valort2)));
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '$') && (flag == 0) && (flag2 == 0))
					{
					if (valort2 != (int) valort2) return STRINGSAIDAERRO;
					valor = roundl ((long double) valort / powl (10, valort2)) * powl (10, valort2);
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '>') && (flag == 0) && (flag2 == 0))
					{
					(valort > valort2) ? (valor = valort) : (valor = valort2);
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '<') && (flag == 0) && (flag2 == 0))
					{
					(valort < valort2) ? (valor = valort) : (valor = valort2);
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == ':') && (flag == 0) && (flag2 == 0))
					{
					valor = logl (valort) / logl (valort2);
					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}

				if ((strt [posicoes_operadores [i]] == '~') && (flag == 0) && (flag2 == 0))
					{
					if (fabsl (logl (valort) - logl (valort2)) < APROXIMACAO)
						valor = 1;
					else
						valor = 0;

					if (valor > VALOR_MAX) return STRINGSAIDAERROOVER;
					break;
					}
				}

		strcpy (strt4, "");

		for (i = 0; i < ponteiroinicio; i++)
			strncat (strt4, & strt [i], 1);

		strcat (strt4, antoniovandre_numeroparastring (valor));

		for (i = ponteirofim; i < strlen (strt); i++)
			strncat (strt4, & strt [i], 1);

		strcpy (strt, strt4);
		}

	return antoniovandre_evalcelulafuncao (strt);
	}

// Função eval.

char * antoniovandre_eval (char * str)
	{
	char str2 [TAMANHO_BUFFER_PHRASE];
	char str2t [TAMANHO_BUFFER_PHRASE];
	char str3 [TAMANHO_BUFFER_PHRASE];
	char str4 [TAMANHO_BUFFER_PHRASE];
	char str5 [TAMANHO_BUFFER_PHRASE];
	char str6 [TAMANHO_BUFFER_WORD];
	int inicio;
	int fim;
	int i;
	int j;
	int k;
	int l;
	int flag;
	int flag2;
	int flag3;
	int flag4;
	int contador;
	char tc;
	char tc2;

	strcpy (str2, "");
	strcpy (str3, "");
	strcpy (str4, "");
	strcpy (str5, "");
	strcpy (str6, "");

	for (i = 0; i < strlen (str); i++)
		if (str [i] != ' ') strncat (str2, & str [i], 1);

	if (! strcmp (str2, "")) return STRINGSAIDAERRO;

	contador = 0;

	for (i = 0; i < strlen (str2); i++)
		{
		flag2 = 0;

		if ((i >= 1) && (str2 [i] == '-') && (str2 [i - 1] == TOKENINICIOEVAL))  flag2 = 1;

		if (((i == 0) && (str2 [0] == '-') || (flag2 == 1)) && ! ((i > 1) && (str2 [i] == '1') && (str2 [i - 1] == '-') && (str2 [i - 2] == TOKENINICIOEVAL)))
			{
			j = i + 1;

			if (str2 [i] == TOKENINICIOEVAL) contador++;
			if (str2 [i] == TOKENFIMEVAL) contador--;

			do
				{
				flag = 0;

				for (k = 0; k < strlen (antoniovandre_numeros); k++)
					for (l = 0; l < strlen (antoniovandre_letras); l++)
						if ((! ((flag2 == 1) && (contador == -1))) && ((str2 [j] == antoniovandre_numeros [k]) || (str2 [j] == antoniovandre_letras [l]) || (str2 [j] == TOKENINICIOEVAL) || (str2 [j] == TOKENFIMEVAL))) flag = 1;

				if (flag == 1) j++;
				} while (flag == 1);

			flag = 0;

			for (k = 0; k < strlen (antoniovandre_operadoresprioritarios); k++)
				if (str2 [j] == antoniovandre_operadoresprioritarios [k])
					flag = 1;

			if (flag == 1)
				{
				strcpy (str2t, "");

				for (k = 0; k < i; k++)
					strncat (str2t, & str2 [k], 1);

				tc = TOKENINICIOEVAL;
				strncat (str2t, & tc, 1);

				strcat (str2t, "-1");

				tc = TOKENFIMEVAL;
				strncat (str2t, & tc, 1);

				tc = OPERADORMULTIPLICACAO;
				strncat (str2t, & tc, 1);

				tc = TOKENINICIOEVAL;
				strncat (str2t, & tc, 1);

				for (k = i + 1; k < j; k++)
					strncat (str2t, & str2 [k], 1);

				tc = TOKENFIMEVAL;
				strncat (str2t, & tc, 1);

				for (k = j; k < strlen (str2); k++)
					strncat (str2t, & str2 [k], 1);

				strcpy (str2, str2t);
				}
			}
		}

	for (i = 1; i < strlen (str2); i++)
		{
		tc = str2 [i - 1];
		tc2 = str2 [i];

		for (j = 0; j < strlen (antoniovandre_operadores); j++)
			{
			if ((tc == '-') && (tc2 == antoniovandre_operadores [j])) return STRINGSAIDAERRO;

			flag = 0;

			for (k = 0; k < strlen (antoniovandre_operadoresespeciais); k++)
				if (tc == antoniovandre_operadoresespeciais [k])
					{
					flag = 1;
					break;
					}

			if (flag == 1) break;

			if ((tc == antoniovandre_operadores [j]) && (tc2 == '-')) return STRINGSAIDAERRO;
			}
		}

	do
		{
		inicio = 0;
		fim = strlen (str2) - 1;
		flag = 0;
		flag2 = 0;

		for (i = 0; i < strlen (str2); i++)
			{
			if (str2 [i] == TOKENINICIOEVAL)
				{
				inicio = i + 1;
				flag = 1;
				}

			if (str2 [i] == TOKENFIMEVAL)
				{
				fim = i - 1;
				flag2 = 1;
				break;
				}
			}

		strcpy (str3, "");

		if (flag == flag2)
			{
			k = inicio - 2;
			flag4 = 0;

			do
				{
				flag3 = 0;
				tc = str2 [k];

				for (j = 0; j < strlen (antoniovandre_numeros); j++)
					if (tc == antoniovandre_numeros [j])
						{
						flag3 = 1;
						flag4 = 1;
						}

				for (j = 0; j < strlen (antoniovandre_letrasmaiusculas); j++)
					if (tc == antoniovandre_letrasmaiusculas [j])
						{
						flag3 = 1;
						flag4 = 1;
						}

				k--;
				} while (flag3 == 1);

			strcpy (str6, "");

			if (flag4 == 1) for (j = k; j < inicio - 1; j++) strncat (str6, & str2 [j], 1);

			if (flag4 == 1)
				for (i = 0; i < k; i++) strncat (str3, & str2 [i], 1);
			else
				for (i = 0; i < inicio - 1; i++) strncat (str3, & str2 [i], 1);

			i = strlen (str6);

			tc = str6 [i - 1];

			if (strcmp (str6, ""))
				{
				strcat (str3, str6);
				if (tc != '-') strcat (str3, "*");
				}

			if (!strcmp (str3, "-")) strcpy (str3, "-1*");

			strcpy (str4, "");

			for (i = inicio; i <= fim; i++)
				strncat (str4, & str2 [i], 1);

			strcpy (str5, antoniovandre_evalcelula (str4));

			if (! strcmp (str5, STRINGSAIDAERRO)) return STRINGSAIDAERRO;
			if (! strcmp (str5, STRINGSAIDAERROOVER)) return STRINGSAIDAERROOVER;

			for (i = 0; i < strlen (str5); i++)
				strncat (str3, & str5 [i], 1);

			for (i = fim + 2; i < strlen (str2); i++)
				strncat (str3, & str2 [i], 1);

			strcpy (str2, str3);
			}
		else
			return STRINGSAIDAERRO;
		} while (! ((flag == 0) && (flag2 == 0)));

	return antoniovandre_evalcelula (str2);
	}

// Derivada em um ponto.

char * antoniovandre_derivada (char * str, long double ponto)
	{
	char str2 [TAMANHO_BUFFER_PHRASE];
	char str3 [TAMANHO_BUFFER_PHRASE];
	long double valorsup;
	long double valorinf;
	int i;
	char * err;

	if (ponto > VALOR_MAX) return STRINGSAIDAERROOVER;

	strcpy (str2, "");

	for (i = 0; i < strlen (str); i++)
		if (str [i] == VARIAVELDESUBSTITUICAO)
			strcat (str2, antoniovandre_numeroparastring ((long double) ((long double) ponto + (long double) EPSILON)));
		else
			strncat (str2, & str [i], 1);

	valorsup = strtold (antoniovandre_eval (str2), & err);

	if (* err != 0) return STRINGSAIDAERRO;

	strcpy (str3, "");

	for (i = 0; i < strlen (str); i++)
		if (str [i] == VARIAVELDESUBSTITUICAO)
			strcat (str3, antoniovandre_numeroparastring ((long double) ((long double) ponto - (long double) EPSILON)));
		else
			strncat (str3, & str [i], 1);

	valorinf = strtold (antoniovandre_eval (str3), & err);

	if (* err != 0) return STRINGSAIDAERRO;

	return antoniovandre_numeroparastring ((long double) ((long double) valorsup - (long double) valorinf) / (long double) (2 * (long double) EPSILON));
	}

// Integral definida.

char * antoniovandre_integraldefinida (char * str, long double a, long double b)
	{
	char str2 [TAMANHO_BUFFER_PHRASE];
	char str3 [TAMANHO_BUFFER_PHRASE];
	long double integral = 0;
	long double norma;
	long double parcela;
	int i;
	int j;
	char * err;

	norma = (long double) ( (long double) b - (long double) a) / (long double) NUMEROPARTICOESSOMARIEMANN;

	for (j = 0; j < NUMEROPARTICOESSOMARIEMANN; j++)
		{
		strcpy (str2, "");

		for (i = 0; i < strlen (str); i++)
			if (str [i] == VARIAVELDESUBSTITUICAO)
				strcat (str2, antoniovandre_numeroparastring ((long double) ((long double) a + (long double) j * (long double) norma + (long double) ((long double) norma / 2))));
			else
				strncat (str2, & str [i], 1);

		strcpy (str3, antoniovandre_eval (str2));

		if (! strcmp (str3, STRINGSAIDAERROOVER)) return STRINGSAIDAERROOVER;

		parcela = strtold (str3, & err) * (long double) norma;

		if (* err != 0) return STRINGSAIDAERRO;

		integral += (long double) parcela;

		if (integral > VALOR_MAX) return STRINGSAIDAERROOVER;
		}

	return antoniovandre_numeroparastring (integral);
	}

// Retorna a função mais próxima, dados os pontos e as funções em arquivos.

// Retorna a função mais próxima, dados os pontos e as funções em arquivos.

char * antoniovandre_funcaomaisproxima (char * arquivopontospath, char * arquivofuncoespath, int log)
	{
	FILE * arquivopontos;
	FILE * arquivofuncoes;
	char buffer [TAMANHO_BUFFER_PHRASE];
	char buffert [TAMANHO_BUFFER_PHRASE];
	char buffertt [TAMANHO_BUFFER_PHRASE];
	char bufferr [TAMANHO_BUFFER_PHRASE];
	char buffer1 [TAMANHO_BUFFER_WORD];
	char buffer2 [TAMANHO_BUFFER_WORD];
	char antoniovandre_funcoes_buffer_char;
	char antoniovandre_pontos_buffer_char;
	unsigned long int totalitens = 0;
	unsigned long int totalpontos = 0;
	unsigned long int totalfuncoes = 0;
	unsigned long int contadoritens = 0;
	long double m = VALOR_MAX;
	long double mt;
	long double x;
	long double y;
	long double yt;
	int i = 1;
	int flag;
	int flag2 = 0;
	int flag3 = 0;
	char tc;
	char * err;

	if (log == 1)
		{
		printf ("Verificando validade dos arquivos... ");
		fflush (stdout);
		}

	arquivopontos = fopen (arquivopontospath, "r");

	if (arquivopontos == NULL)
		{
		if (log == 1) printf ("Erro.\n");
		return STRINGSAIDAERRO;
		}

	fseek (arquivopontos, 0, SEEK_END);

	if (ftell (arquivopontos) > (unsigned long int) TAMANHO_MAX_ARQUIVO)
		{
		if (log == 1) printf ("Erro.\n");
		fclose (arquivopontos);
		return STRINGSAIDAERROOVER;
		}

	fseek (arquivopontos, 0, SEEK_SET);

	arquivofuncoes = fopen (arquivofuncoespath, "r");

	if (arquivofuncoes == NULL)
		{
		if (log == 1) printf ("Erro.\n");
		return STRINGSAIDAERRO;
		}

	fseek (arquivofuncoes, 0, SEEK_END);

	if (ftell (arquivofuncoes) > (unsigned long int) TAMANHO_MAX_ARQUIVO)
		{
		if (log == 1) printf ("Erro.\n");
		fclose (arquivofuncoes);
		return STRINGSAIDAERROOVER;
		}

	fseek (arquivofuncoes, 0, SEEK_SET);

	if (log == 1) printf("Ok.\n");

	if (log == 1)
		{
		printf ("Reunindo metadados...  ");
		fflush (stdout);
		}

	do
		{
		antoniovandre_pontos_buffer_char = fgetc (arquivopontos);

		if ((antoniovandre_pontos_buffer_char == DELIMITADORSTRING2) || (antoniovandre_pontos_buffer_char == '\n') || (feof (arquivopontos))) totalpontos++;

		if (i++ % INTERVALOPROGRESSO2 == 0)
			{
			switch (tc)
				{
				case '-':
					tc = '\\';
					break;
				case '\\':
					tc = '|';
					break;
				case '|':
					tc = '/';
					break;
				case '/':
					tc = '-';
					break;
				default:
					tc = '-';
				}

			printf ("\b%c", tc);
			fflush (stdout);

			i = 1;
			}
		} while (! feof (arquivopontos));

	if ((antoniovandre_pontos_buffer_char == DELIMITADORSTRING2) || (antoniovandre_pontos_buffer_char == '\n')) totalpontos--;

	do
		{
		antoniovandre_funcoes_buffer_char = fgetc (arquivofuncoes);

		if ((antoniovandre_funcoes_buffer_char == DELIMITADORSTRING) || (antoniovandre_funcoes_buffer_char == '\n') || (feof (arquivofuncoes))) totalfuncoes++;

		if (i++ % INTERVALOPROGRESSO2 == 0)
			{
			switch (tc)
				{
				case '-':
					tc = '\\';
					break;
				case '\\':
					tc = '|';
					break;
				case '|':
					tc = '/';
					break;
				case '/':
					tc = '-';
					break;
				default:
					tc = '-';
				}

			printf ("\b%c", tc);
			fflush (stdout);

			i = 1;
			}
		} while (! feof (arquivofuncoes));

	if ((antoniovandre_funcoes_buffer_char == DELIMITADORSTRING) || (antoniovandre_funcoes_buffer_char == '\n')) totalfuncoes--;

	totalitens = totalpontos * totalfuncoes;

	if (log == 1) printf("\bOk.\n");

	fseek (arquivopontos, 0, SEEK_SET);
	fseek (arquivofuncoes, 0, SEEK_SET);

	strcpy (bufferr, "");

	do
		{
		if ((flag2 == 1) || (flag3 == 0))
			{
			strcpy (buffer, "");

			do
				{
				antoniovandre_funcoes_buffer_char = fgetc (arquivofuncoes);

				if ((! feof (arquivofuncoes)) && (antoniovandre_funcoes_buffer_char != DELIMITADORSTRING) && (antoniovandre_funcoes_buffer_char != ' ') && (antoniovandre_funcoes_buffer_char != '\n'))
					strncat (buffer, & antoniovandre_funcoes_buffer_char, 1);
				} while (((! feof (arquivofuncoes))) && (antoniovandre_funcoes_buffer_char != DELIMITADORSTRING) && (antoniovandre_funcoes_buffer_char != '\n'));

			fseek (arquivopontos, 0, SEEK_SET);
			flag2 = 0;
			flag3 = 1;
			mt = 0;
			}

		strcpy (buffer1, "");
		strcpy (buffer2, "");
		flag = 0;

		while (flag2 == 0)
			{
			antoniovandre_pontos_buffer_char = fgetc (arquivopontos);

			if (antoniovandre_pontos_buffer_char != ' ')
				if (antoniovandre_pontos_buffer_char == DELIMITADORSTRING)
					flag = 1;
				else
					{
					if (feof (arquivopontos))
						{
						flag2 = 1;
						break;
						}
					else
						{
						if ((antoniovandre_pontos_buffer_char == DELIMITADORSTRING2) || (antoniovandre_pontos_buffer_char == '\n'))
							{
							flag = 0;
							break;
							}
						else
							{
							if (flag == 0)
								strncat (buffer1, & antoniovandre_pontos_buffer_char, 1);
							else
								strncat (buffer2, & antoniovandre_pontos_buffer_char, 1);
							}
						}
					}
			}

		x = strtold (buffer1, & err);

		if (* err != 0) return STRINGSAIDAERRO;
		if (x > VALOR_MAX) return STRINGSAIDAERROOVER;

		y = strtold (buffer2, & err);
			
		if (y > VALOR_MAX) return STRINGSAIDAERROOVER;
		if (* err != 0) return STRINGSAIDAERRO;

		strcpy (buffert, "");

		for (i = 0; i < strlen (buffer); i++)
			{
			tc = buffer [i];

			if (tc == 'x')
				strcat (buffert, antoniovandre_numeroparastring ((long double) x));
			else
				strncat (buffert, & tc, 1);
			}

		strcpy (buffertt, antoniovandre_eval (buffert));

		if (! strcmp (buffertt, STRINGSAIDAERROOVER)) return STRINGSAIDAERROOVER;

		yt = strtold (buffertt, & err);

		contadoritens++;

		if (* err == 0) mt += (long double) ((long double) y - (long double) yt); else {flag2 = 1; continue;}

		if ((flag2 == 1) && (fabs ((long double) mt) < (long double) m))
			{
			m = fabs ((long double) mt);
			strcpy (bufferr, buffer);
			}

		if ((log == 1) && ((contadoritens == 1) || (contadoritens == totalitens - totalpontos) || (contadoritens % INTERVALOPROGRESSO == 0)))
			{
			printf ("\r%.13f%% concluído.", (double) (100.0 * (double) ((double) (contadoritens) / (double) (totalitens - (double) totalpontos))));

			fflush (stdout);
			}

		} while ((! feof (arquivofuncoes)) || (flag2 == 0));

	if (log == 1) {printf ("\n"); fflush (stdout);}

	fclose (arquivopontos);
	fclose (arquivofuncoes);

	return bufferr;
	}

// Produto de números complexos.

numerocomplexo antoniovandre_produtocomplexo (numerocomplexo * numeroscomplexos, int numeroargumentos)
	{
	numerocomplexo result;
	long double a;
	long double b;
	int i;

	a = 1;
	b = 0;

	for (i = 0; i < numeroargumentos; i++)
		{
		result.real = a * numeroscomplexos [i].real - b * numeroscomplexos [i].img;

		result.img = b * numeroscomplexos [i].real + a * numeroscomplexos [i].img;

		a = result.real; b = result.img;
		}

	return result;
	}

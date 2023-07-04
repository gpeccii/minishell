/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:39:25 by dcologgi          #+#    #+#             */
/*   Updated: 2023/06/27 10:51:33 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

int isDelimiter(char c, char *str, int i) 
{
	return (c == '|' || c == '&');
}

char *insertSpaces(char* str, int len) {
	int i, j;
	int newLen = len;
	int withinQuotes = 0;
	int withinDoubleQuotes = 0;
	int quotesStack = 0;
	int doubleQuotesStack = 0;

	// Contare il numero di spazi necessari
	for (i = 0; i < len; i++) {
		if (str[i] == '\'') {
			withinQuotes = !withinQuotes;
			if (withinQuotes) {
				quotesStack++;
			} else {
				quotesStack--;
			}
		} else if (str[i] == '\"') {
			withinDoubleQuotes = !withinDoubleQuotes;
			if (withinDoubleQuotes) 
			{
				doubleQuotesStack++;
			} 
			else 
			{
				doubleQuotesStack--;
			}
		} else if (!withinQuotes && !withinDoubleQuotes && isDelimiter(str[i], str, i)) {
			newLen++;
		}
	}

	// Se gli apici non sono bilanciati, correggere il numero di spazi
	if (quotesStack != 0) 
	{
		newLen -= quotesStack;
	}
	else
		newLen = newLen;
	if (doubleQuotesStack != 0) 
	{
		newLen -= doubleQuotesStack;
	}
	else
		newLen = newLen;
	// Creare una nuova stringa con gli spazi
	char* newStr = ft_calloc(newLen +2, sizeof(char));
	j = 0;
	withinQuotes = 0;
	withinDoubleQuotes = 0;
	for (i = 0; i < len; i++) {
		if (str[i] == '\'') {
			withinQuotes = !withinQuotes;
		} else if (str[i] == '\"') {
			withinDoubleQuotes = !withinDoubleQuotes;
		}

		if (!withinQuotes && !withinDoubleQuotes && isDelimiter(str[i], str, i)) //no virgolette
		{
			if (newStr[j] != '|')
				newStr[j++] = ' ';
			newStr[j++] = str[i];
			//if (str[i] == '&') 
			//{
			//	newStr[j++] = '&';
			//	i++;  // Saltare il secondo carattere '&'
			//}
			if (str[i] == '|') 
			{
				//newStr[j] = '|';
				if (newStr[j + 1] == '|' && newStr[j - 1] != '|')
				{
					newStr[++j] = '|';
					j++;
				}
				////printf("newstr == %s\n", newStr);
			}
			//newStr[j++] = ' ';
		} 
		else 
		{
			if (str[i] == '&') 
			{
				newStr[j++] = '&';
				i++;  // Saltare il secondo carattere '&'
			}
			if (str[i] == '|') 
			{
				newStr[j] = '|';
				if (newStr[j + 1] == '|')
				{
					newStr[++j] = '|';
					j++;
				}
				////printf("newnewstr == %s\n", newStr);
			}
			newStr[j++] = str[i];
		}
		////printf("str[%d] = %c\n", i, str[i]);
		////printf("new str[%d] = %s\n", i, newStr);

	}
	newStr[j] = '\0';
	////printf("Stringa con spazi: %s\n", newStr);
	return (newStr);
	// Liberare la memoria allocata
}

int start(t_shell *mini) 
{
	// Rimuovere il carattere di nuova riga finale se presente
	int len = strlen(mini->input);
	if (len == 0 || !mini->input)
		return (0);
	if (len > 0 && mini->input[len - 1] == '\n') {
		mini->input[len - 1] = '\0';
		len--;
	}

	// Chiamare la funzione per inserire gli spazi
	mini->input = insertSpaces(mini->input, len);
	mini->input = getting_final_string(mini->input, '|');
	mini->input = getting_final_string(mini->input, '&');
	mini->input = getting_final_string(mini->input, '<');
	mini->input = getting_final_string(mini->input, '>');
	return (1);
}

char *getting_final_string(char *s, char c)
{
	
	char *d = ft_calloc(ft_strlen(s) + counting(s) + 1, sizeof(char));
	int i = 0;
	int j = 0;
	int z = 0;
	while (s[i])
	{
		if (s[i] == c && c != '<' && c != '>')
		{
			d[z++] = s[i];

			j = i + 1;

			while (s[j] == ' ')
			{
				if (s[j] == '\0')
					break;
				j++;
			}
			if (s[j] != c)
			{
				d[z++] = ' ';
				i = j;
			}
			else
			{
				i = j;
				d[z++] = s[i++];
				d[z++] = ' ';
			}
		}
		else if (s[i] == '<' || s[i] == '>')
		{
			d[z++] = ' ';
			if (s[i + 1] != ' ' )
			{
				d[z++] = s[i++];
				if (s[i] == '<' || s[i] == '>')
					d[z++] = s[i++];
				if (s[i] != '<' && s[i] != '>')
					d[z++] = ' ';
			}
		}
		d[z] = s[i];
		i++;
		z++;
	}
	d[z] = '\0';
	free(s);
	return (d);
	
}

int	counting(char *s)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '|' || s[i] == '&' || s[i] == '<' || s[i] == '>')
		{
			if (checking_quotes_for_operator(s, s[i], i))
				k++;
		}
		i++;
	}
	return (k);
}
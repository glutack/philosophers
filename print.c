/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irmoreno <irmoreno@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 17:33:10 by irmoreno          #+#    #+#             */
/*   Updated: 2023/05/08 22:25:55 by irmoreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*	Frees all memory allocated */
void	*ft_clean_table(t_program *tools)
{
	unsigned int	i;

	if (!tools)
		return (NULL);
	if (tools->forks_mutex != NULL)
		free(tools->forks_mutex);
	if (tools->philos != NULL)
	{
		i = 0;
		while (i < tools->nbphilos)
		{
			if (tools->philos[i] != NULL)
				free(tools->philos[i]);
			i++;
		}
		free(tools->philos);
	}
	free(tools);
	return (NULL);
}

void	ft_actions(t_philo *philo, int status, char *action)
{
	pthread_mutex_lock(&philo->tools->actions_mutex);
	if (!ft_routine_status(philo->tools) && (status == 0 || status == 2
			|| status == 3))
	{
		pthread_mutex_unlock(&philo->tools->actions_mutex);
		return ;
	}
	if (status == 2)
		printf("%ld %d %s %d\n", (ft_get_time() - philo->tools->tstart),
			philo->id + 1, "has taken fork", philo->fork[0]);
	else if (status == 3)
		printf("%ld %d %s %d\n", (ft_get_time() - philo->tools->tstart),
			philo->id + 1, "has taken fork", philo->fork[1]);
	else
		printf("%ld %d %s\n", (ft_get_time() - philo->tools->tstart),
			philo->id + 1, action);
	pthread_mutex_unlock(&philo->tools->actions_mutex);
}

int	ft_error(char *str, int error)
{
	printf("%s", str);
	return (error);
}

t_philo	**ft_tphilo_error(char *str, void *error)
{
	printf("%s", str);
	return (error);
}

pthread_mutex_t	*ft_mutex_error(char *str, void *error)
{
	printf("%s", str);
	return (error);
}

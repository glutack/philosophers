/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irmoreno <irmoreno@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 21:14:18 by irmoreno          #+#    #+#             */
/*   Updated: 2023/05/08 13:38:21 by irmoreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*	Creates a small delay at the beginning of each philosophers routine execution
	so that all threads start at the same time with the same start time
	reference so the philo father is synchronized with the philosophers. */
void	ft_routine_delay(time_t tstart)
{
	while (ft_get_time() < tstart)
		continue ;
}

static void	ft_eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->tools->forks_mutex[philo->fork[0]]);
	ft_actions(philo, 2, "fork");
	pthread_mutex_lock(&philo->tools->forks_mutex[philo->fork[1]]);
	ft_actions(philo, 3, "fork");
	ft_actions(philo, 0, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = ft_get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_philo_usleep(philo->tools, philo->tools->teat); //no
	if (ft_routine_status(philo->tools) == 1)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		philo->nbmeals += 1;
		pthread_mutex_unlock(&philo->meal_mutex);
	}
	ft_actions(philo, 0, "is sleeping");
	pthread_mutex_unlock(&philo->tools->forks_mutex[philo->fork[1]]);
	pthread_mutex_unlock(&philo->tools->forks_mutex[philo->fork[0]]);
	ft_philo_usleep(philo->tools, philo->tools->tsleep); //no
}

static void	ft_think_routine(t_philo *philo, int status)
{
	time_t	tthink;

	pthread_mutex_lock(&philo->meal_mutex);
	tthink = (philo->tools->tdie - (ft_get_time() - philo->last_meal)
			- philo->tools->teat) / 2;
	pthread_mutex_unlock(&philo->meal_mutex);
	if (tthink < 0)
		tthink = 0;
	if (tthink == 0 && status == 1)
		tthink = 1;
	if (tthink > 600)
		tthink = 200;
	if (status == 0)
		ft_actions(philo, 0, "is thinking");
	ft_philo_usleep(philo->tools, tthink); //no
}

/*	If there's one single philosopher, he will only have one fork, so he will
	not be able to eat, so he will pick up the only fork available and die */
static void	*ft_one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->tools->forks_mutex[philo->fork[0]]);
	ft_actions(philo, 2, "fork");
	ft_philo_usleep(philo->tools, philo->tools->tdie);
	ft_actions(philo, 0, "died");
	pthread_mutex_unlock(&philo->tools->forks_mutex[philo->fork[0]]);
	return (NULL);
}

/**/
void	*ft_philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->tools->nbmeals == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = philo->tools->tstart;
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_routine_delay(philo->tools->tstart); //no
	if (philo->tools->tdie == 0)
		return (NULL);
	if (philo->tools->nbphilos == 1)
		return (ft_one_philo_routine(philo));
	else if (philo->id % 2)
		ft_think_routine(philo, 0);
	while (ft_routine_status(philo->tools) == 1)
	{
		ft_eat_sleep_routine(philo);
		ft_think_routine(philo, 1);
	}
	return (NULL);
}
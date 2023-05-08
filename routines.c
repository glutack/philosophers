/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irmoreno <irmoreno@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 21:14:18 by irmoreno          #+#    #+#             */
/*   Updated: 2023/05/08 22:25:39 by irmoreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*	Creates a small delay at the beginning of each philosophers routine execution
	so that all threads start at the same time with the same start time
	reference so the philo father is synchronized with the philosophers. It
	calls FT_GET_TIME*/
void	ft_routine_delay(time_t tstart)
{
	while (ft_get_time() < tstart)
		continue ;
}

/*	Locks the forks the philo had assigned, updates it's last meal and he'll
	FT_PHILO_USLEEP for the time to eat given in the parameters, then he'll 
	FT_PHILO_USLEEP for the time to sleep given in the parameters */
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
	ft_philo_usleep(philo->tools, philo->tools->teat);
	if (ft_routine_status(philo->tools) == 1)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		philo->nbmeals += 1;
		pthread_mutex_unlock(&philo->meal_mutex);
	}
	ft_actions(philo, 0, "is sleeping");
	pthread_mutex_unlock(&philo->tools->forks_mutex[philo->fork[1]]);
	pthread_mutex_unlock(&philo->tools->forks_mutex[philo->fork[0]]);
	ft_philo_usleep(philo->tools, philo->tools->tsleep);
}

/*	Time to think (tthink) will be calculated by deducting the time to die
	- the actual time - this philos last meal - time to eat/2, that way it'll
	stop thinkin around the same time another philosopher finishes eating.
	If tthink is negative, it means	there's no time to think, if it's 0, we'll
	have a small wait but he won't start thiking and if it's +600 we'll reduce
	it to 200 as there's no way it'll need that much time to wait, then he'll
	FT_PHILO_USLEEP for	the amount of time calculated */
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
	ft_philo_usleep(philo->tools, tthink);
}

/*	If there's one single philosopher, he will only have one fork and will
	not be able to eat, so he will pick up the only fork available and die. */
static void	*ft_one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->tools->forks_mutex[philo->fork[0]]);
	ft_actions(philo, 2, "fork");
	ft_philo_usleep(philo->tools, philo->tools->tdie);
	ft_actions(philo, 0, "died");
	pthread_mutex_unlock(&philo->tools->forks_mutex[philo->fork[0]]);
	return (NULL);
}

/*	If tools->nbmeals = 0 || tools->tdie == 0 lunch doesn't start as
	philosophers won't be able to eat. States the starting time as the last
	meal to have a reference for future meals and calls FT_RUTINE_DELAY.
	If tools->nbphilos == 1 it calls FT_ONE_PHILO_ROUTINE, if > 1, it makes all even
	philosophers to start thinking (FT_THINK_ROUTINE) so the rest will start
	FT_EAT_SLEEP_ROUTINE while FT_ROUTINE_STATUS checks that routine is active.
	All routines call FT_ACTIONS*/
void	*ft_philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->tools->nbmeals == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = philo->tools->tstart;
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_routine_delay(philo->tools->tstart);
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
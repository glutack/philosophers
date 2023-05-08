/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irmoreno <irmoreno@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 21:53:59 by irmoreno          #+#    #+#             */
/*   Updated: 2023/05/08 14:51:41 by irmoreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_lunch(t_program *tools)
{
	unsigned int	i;

	tools->tstart = ft_get_time();
	i = 0;
	while (i < tools->nbphilos)
	{
		if (pthread_create(&tools->philos[i]->thread, NULL, &ft_philo_routine,
				tools->philos[i]) != 0)
			return (ft_error("source: init philos lunch\nerror: thread \
creation\n", 0));
		i++;
	}
	if (tools->nbphilos > 1)
	{
		if (pthread_create(&tools->philo_father, NULL, &ft_philo_father, tools)
			!= 0)
			return (ft_error("source: init philo father\nerror: thread \
creation\n", 0));
	}
	return (1);
}

pthread_mutex_t	*ft_init_forks(t_program *tools)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * tools->nbphilos);
	if (!forks)
		return (ft_mutex_error("source: forks\nerror: memory allocation\n",
				NULL));
	i = 0;
	while (i < tools->nbphilos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (ft_mutex_error("source: meal_mutex\nerror: mutex creation\n",
					NULL));
		i++;
	}
	return (forks);
}

/* 	Initializes forks for every philosopher. Even-numbered philosophers will try
	to take fork(id + 1) first so there's always two consecutive forks free */
static void	ft_allocate_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->tools->nbphilos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->tools->nbphilos;
		philo->fork[1] = philo->id;
	}
}

/*	Allocates memory and initializes philosophers */
static t_philo	**ft_init_philos(t_program *tools)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * tools->nbphilos);
	if (!philos)
		return (ft_tphilo_error("source: philos\nerror: memory allocation\n",
				NULL));
	i = 0;
	while (i < tools->nbphilos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (ft_tphilo_error("source: philos\nerror: memory allocation\n",
					NULL));
		if (pthread_mutex_init(&philos[i]->meal_mutex, 0) != 0)
			return (ft_tphilo_error("source: philos->meal_mutex\nerror: mutex \
creation\n", NULL));
		philos[i]->tools = tools;
		philos[i]->id = i;
		philos[i]->nbmeals = 0;
		ft_allocate_forks(philos[i]);
		i++;
	}
	return (philos);
}

/*	Initializes the program that contains the parameters received. */
t_program	*ft_init_tools(t_program *tools, int argc, char **argv)
{
	tools = malloc(sizeof(t_program) * 1);
	if (!tools)
		return (NULL);
	tools->nbphilos = ft_atoi(argv[1]);
	tools->tdie = ft_atoi(argv[2]);
	tools->teat = ft_atoi(argv[3]);
	tools->tsleep = ft_atoi(argv[4]);
	tools->nbmeals = -1;
	if (argc == 6)
		tools->nbmeals = ft_atoi(argv[5]);
	tools->philos = ft_init_philos(tools);
	if (!tools->philos)
		return (NULL);
	tools->forks_mutex = ft_init_forks(tools);
	tools->routine_status = 1;
	return (tools);
}

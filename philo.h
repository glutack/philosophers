/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irmoreno <irmoreno@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:45:50 by irmoreno          #+#    #+#             */
/*   Updated: 2023/05/04 13:45:50 by irmoreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <string.h> //memset

typedef struct s_philo	t_philo;

typedef struct s_program {
	unsigned int	nbphilos;
	time_t			tdie;
	time_t			teat;
	time_t			tsleep;
	int				nbmeals;
	pthread_mutex_t	*forks_mutex;
	time_t			tstart;
	pthread_t		philo_father;
	pthread_mutex_t	actions_mutex;
	int				routine_status;
	pthread_mutex_t	routine_status_mutex;
	t_philo			**philos;
}					t_program;

typedef struct s_philo {
	pthread_t		thread;
	unsigned int	id;
	int				nbmeals;
	unsigned int	fork[2];
	pthread_mutex_t	meal_mutex;
	time_t			last_meal;
	t_program		*tools;
}						t_philo;

// UTILS
int				ft_check_arg(int argc, char **argv);
int				ft_atoi(const char *str);
time_t			ft_get_time(void);
int				ft_routine_status(t_program *tools);
void			ft_philo_usleep(t_program *tools, time_t tsleep);

// PRINT
void			ft_actions(t_philo *philo, int status, char *action);
int				ft_error(char *str, int error);
t_philo			**ft_tphilo_error(char *str, void *error);
pthread_mutex_t	*ft_mutex_error(char *str, void *error);

// INIT
t_program		*ft_init_tools(t_program *tools, int argc, char **argv);
int				ft_init_lunch(t_program *tools);

// ROUTINES
void			ft_routine_delay(time_t tstart);
void			*ft_philo_routine(void *data);

// PHILO FATHER
void			*ft_philo_father(void *data);

// MEMORY CLEANSE (print.c)
void			*ft_clean_table(t_program *tools);
#endif

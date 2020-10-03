/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelessa <cool.3meu@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 19:54:03 by dmelessa          #+#    #+#             */
/*   Updated: 2020/10/02 22:04:03 by dmelessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "app.h"

static void	set_kernel_args(t_rt rt, int step)
{
	static uint32_t a;
	cl_kernel	k;
	int			err;

	k = rt.ocl_program.new_kernel;
	err = 0;
	err |= clSetKernelArg(k, 0, sizeof(cl_mem), &rt.ocl_program.rgb_image);
	err |= clSetKernelArg(k, 1, sizeof(cl_int), &step);
	err |= clSetKernelArg(k, 2, sizeof(cl_mem), &rt.ocl_program.instances);
	err |= clSetKernelArg(k, 3, sizeof(cl_int),
							&rt.scene.instance_mngr.ninstances);
	err |= clSetKernelArg(k, 4, sizeof(cl_mem), &rt.ocl_program.objects);
	err |= clSetKernelArg(k, 5, sizeof(cl_int),
							&rt.scene.instance_mngr.nobjects);
	err |= clSetKernelArg(k, 6, sizeof(cl_mem), &rt.ocl_program.triangles);
	err |= clSetKernelArg(k, 7, sizeof(cl_int),
							&rt.scene.instance_mngr.ntriangles);
	err |= clSetKernelArg(k, 8, sizeof(cl_mem), &rt.ocl_program.matrices);
	err |= clSetKernelArg(k, 9, sizeof(cl_int),
							&rt.scene.instance_mngr.nmatrices);
	err |= clSetKernelArg(k, 10, sizeof(cl_mem), &rt.ocl_program.lights);
	err |= clSetKernelArg(k, 11, sizeof(cl_int), &rt.scene.nlights);
	err |= clSetKernelArg(k, 12, sizeof(t_camera), &rt.scene.camera);
	err |= clSetKernelArg(k, 13, sizeof(t_light), &rt.scene.ambient_light);
	err |= clSetKernelArg(k, 14, sizeof(t_ambient_occluder),
							&rt.scene.ambient_occluder);
	err |= clSetKernelArg(k, 15, sizeof(cl_mem), &rt.ocl_program.bvh);
	err |= clSetKernelArg(k, 16, sizeof(t_rt_options), &rt.options);
	err |= clSetKernelArg(k, 17, sizeof(cl_mem), &rt.ocl_program.samplers);
	err |= clSetKernelArg(k, 18, sizeof(cl_mem), &rt.ocl_program.samples);
	err |= clSetKernelArg(k, 19, sizeof(cl_mem), &rt.ocl_program.disk_samples);
	err |= clSetKernelArg(k, 20, sizeof(cl_mem),
							&rt.ocl_program.hemisphere_samples);
	err |= clSetKernelArg(k, 21, sizeof(uint32_t), &a);
	a++;
	assert(!err);
}


void	render_scene(t_rt rt)
{
	int	err;
	int	i;
	i = 0;
	while (i < NUM_SAMPLES)
	{
		set_kernel_args(rt, i);
		err = clEnqueueNDRangeKernel(rt.ocl_program.info.queue,
									 rt.ocl_program.new_kernel,
									 1, NULL,
									 &rt.ocl_program.work_size,
									 &rt.ocl_program.work_group_size,
									 0, NULL, NULL);
		cl_error(&rt.ocl_program, &rt.ocl_program.info, err);
		assert(!err);
		i++;
		// printf("RENDERING: %f\n", 100.0f * i / NUM_SAMPLES);
	}
}

int		init_rt(t_rt *rt, char *scene_file)
{
	init_sampler_manager(&rt->sampler_manager);
	init_default_options(&rt->options, &rt->sampler_manager);
	init_default_scene(&rt->scene, &rt->sampler_manager);
	rt->options.ambient_occluder_sampler
		= rt->sampler_manager.samplers[rt->scene.ambient_occluder.sampler_id];
	rt->scene.bvh = build_bvh(&rt->scene);
	init_ocl(&rt->ocl_program, &rt->scene, &rt->sampler_manager);
}

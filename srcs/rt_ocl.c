/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_ocl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelessa <cool.3meu@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 18:59:58 by dmelessa          #+#    #+#             */
/*   Updated: 2020/09/28 14:44:59 by dmelessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_ocl.h"

#include <assert.h>

static int	init_clp(t_clp *clp)
{
	cl_platform_id	pl_id;
	cl_uint			nde;

	pl_id = NULL;
	clp->de_id = NULL;
	clp->ret = clGetPlatformIDs(1, &pl_id, NULL);
	assert(!clp->ret);
	clp->ret = clGetDeviceIDs(pl_id, CL_DEVICE_TYPE_ALL, 1, &clp->de_id, &nde);
	assert(!clp->ret);
	clp->context = clCreateContext(NULL, 1, &clp->de_id, NULL, NULL, &clp->ret);
	assert(!clp->ret);
	clp->queue = clCreateCommandQueue(clp->context, clp->de_id,
		CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &clp->ret);
	assert(!clp->ret);
	return (0);
}

static int	init_kernel(t_cl_program *p)
{
	int	r;

	r = 0;
	p->work_size = DEFAULT_WORK_SIZE;
	p->work_group_size = WORK_GROUP_SIZE;
	p->program = create_program(p->info.context);
	r = clBuildProgram(p->program, 1, &p->info.de_id, KERNEL_INC, NULL, NULL);
	cl_error(p, &p->info, r);
	assert(!r);
	p->new_kernel = clCreateKernel(p->program, KERNEL_NAME, &r);
	assert(!r);
	p->help_kernel = clCreateKernel(p->program, "translate_image", &r);
	assert(!r);
	return (r);
}

//NOTE: deal with case when there is no objects
//TODO: mem manager like sampler_manager but for scene
void init_buffers(t_cl_program *program, t_scene *scene,
				  t_sampler_manager *sampler_manager)
{
	int ret;
	int ro;
	cl_context cntx;

	ro = CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR;
	cntx = program->info.context;
	program->rgb_image = clCreateBuffer(cntx, CL_MEM_READ_WRITE, sizeof(t_color) * program->work_size, NULL, &ret);
	cl_error(program, &program->info, ret);
	program->output_image = clCreateBuffer(cntx, CL_MEM_READ_WRITE,
										   sizeof(uint32_t) * program->work_size, NULL, &ret);
	cl_error(program, &program->info, ret);

	program->instances = clCreateBuffer(cntx, ro, sizeof(t_instance) * (scene->instance_mngr.ninstances), scene->instance_mngr.instances, &ret);
	cl_error(program, &program->info, ret);

	program->objects = clCreateBuffer(cntx, ro, sizeof(t_obj) * (scene->instance_mngr.nobjects), scene->instance_mngr.objects, &ret);
	cl_error(program, &program->info, ret);

	program->triangles = clCreateBuffer(cntx, ro, sizeof(t_triangle) * (scene->instance_mngr.ntriangles + 1), scene->instance_mngr.triangles, &ret);
	cl_error(program, &program->info, ret);

	program->matrices = clCreateBuffer(cntx, ro, sizeof(t_matrix) * (scene->instance_mngr.nmatrices), scene->instance_mngr.matrices, &ret);
	cl_error(program, &program->info, ret);

	program->bvh = clCreateBuffer(cntx, ro, sizeof(t_bvh_node) * (scene->instance_mngr.ninstances * 2), scene->bvh, &ret);
	cl_error(program, &program->info, ret);

	program->lights = clCreateBuffer(cntx, ro, sizeof(t_light) * scene->nlights, scene->lights, &ret);
	cl_error(program, &program->info, ret);

	program->samplers = clCreateBuffer(cntx, ro, sizeof(t_sampler) * sampler_manager->count, sampler_manager->samplers, &ret);
	cl_error(program, &program->info, ret);

	program->samples = clCreateBuffer(cntx, ro, sizeof(cl_float2) * sampler_manager->samples_size, sampler_manager->samples, &ret);
	cl_error(program, &program->info, ret);

	program->disk_samples = clCreateBuffer(cntx, ro, sizeof(cl_float2) * sampler_manager->samples_size, sampler_manager->disk_samples, &ret);
	cl_error(program, &program->info, ret);

	program->hemisphere_samples = clCreateBuffer(cntx, ro, sizeof(cl_float3) * sampler_manager->samples_size, sampler_manager->hemisphere_samples, &ret);

	cl_error(program, &program->info, ret);
}

int		init_ocl(t_cl_program *program, t_scene *scene,
				t_sampler_manager *sampler_manager)
{
	init_clp(&program->info);
	init_kernel(program);
	init_buffers(program, scene, sampler_manager);
}
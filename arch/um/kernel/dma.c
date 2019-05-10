#include <linux/types.h>
#include <linux/dma-mapping.h>
#include <linux/dma-direct.h>

const struct dma_map_ops um_dma_ops = {
#if 0
	.map_sg = um_map_sg,
	.unmap_sg = um_unmap_sg,
	.map_page = um_map_page,
	.unmap_page = um_unmap_page,
	.alloc = um_alloc_coherent,
	.free = um_free_coherent,
#endif
	.dma_supported = dma_direct_supported,
};

const struct dma_map_ops *dma_ops = &um_dma_ops;

bool arch_dma_alloc_attrs(struct device **dev)
{
	if (!*dev)
		return false;

	if (!is_device_dma_capable(*dev))
		return false;
	return true;

}
EXPORT_SYMBOL(arch_dma_alloc_attrs);

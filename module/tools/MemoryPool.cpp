#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t max_blocks) 
	: max_blocks_(max_blocks) {
	for (size_t i = 0; i < 1024; ++i) {
		blocks_.emplace_back(std::make_unique<MemoryBlock>());
	}
}

std::unique_ptr<MemoryBlock> MemoryPool::allocate() {
    std::lock_guard<std::mutex> lock(mutex_);
	if (blocks_.empty()) {
        return std::make_unique<MemoryBlock>();
	}
	auto block = std::move(blocks_.front());
    blocks_.pop_front();
	return block;
}

void MemoryPool::deallocate(std::unique_ptr<MemoryBlock> block) {
	std::lock_guard<std::mutex> lock(mutex_);
    if (block && blocks_.size() < max_blocks_) {
        blocks_.push_back(std::move(block)); // 显式移交所有权
    }
    // block 离开作用域自动释放（若未被接收）
}

void MemoryPool::deallocate(std::list<std::unique_ptr<MemoryBlock>> blocks) {
    std::lock_guard<std::mutex> lock(mutex_);
    // 拼接链表
    blocks_.splice(blocks_.end(), blocks);

    if (blocks_.size() > max_blocks_) {
        // 计算需释放的多余元素数量
        size_t excess = blocks_.size() - max_blocks_;

        auto it = blocks_.end();
        std::advance(it, -excess);   // 定位到需保留的末尾位置
        blocks_.erase(it, blocks_.end()); // 删除多余元素
    }
}

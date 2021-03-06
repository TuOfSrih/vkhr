#include <vkpp/queue.hh>

#include <vkpp/exception.hh>

namespace vkpp {
    Queue::Queue(const VkQueue& queue, std::uint32_t family_index)
                : family_index { family_index }, handle { queue } { }

    VkQueue& Queue::get_handle() {
        return handle;
    }

    std::uint32_t Queue::get_family_index() const {
        return family_index;
    }

    Queue& Queue::submit(CommandBuffer& command_buffer) {
        VkSubmitInfo submit_info {  };
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = nullptr;

        submit_info.waitSemaphoreCount = 0;
        submit_info.pWaitSemaphores = nullptr;

        submit_info.pWaitDstStageMask = nullptr;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer.get_handle();

        submit_info.signalSemaphoreCount = 0;
        submit_info.pSignalSemaphores = nullptr;

        if (VkResult error = vkQueueSubmit(handle, 1, &submit_info, VK_NULL_HANDLE)) {
            throw Exception { error, "couldn't submit command buffer to the queue!" };
        }

        return *this;
    }

    Queue& Queue::submit(CommandBuffer& command_buffer,
                         Semaphore& wait,
                         VkPipelineStageFlags wait_stage,
                         Semaphore& signal) {
        VkSubmitInfo submit_info {  };
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = nullptr;

        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &wait.get_handle();

        submit_info.pWaitDstStageMask = &wait_stage;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer.get_handle();

        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &signal.get_handle();

        if (VkResult error = vkQueueSubmit(handle, 1, &submit_info, VK_NULL_HANDLE)) {
            throw Exception { error, "couldn't submit command buffer to the queue!" };
        }

        return *this;
    }

    Queue& Queue::submit(CommandBuffer& command_buffer,
                         Semaphore& wait,
                         Semaphore& signal) {
        VkSubmitInfo submit_info {  };
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = nullptr;

        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &wait.get_handle();

        submit_info.pWaitDstStageMask = nullptr;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer.get_handle();

        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &signal.get_handle();

        if (VkResult error = vkQueueSubmit(handle, 1, &submit_info, VK_NULL_HANDLE)) {
            throw Exception { error, "couldn't submit command buffer to the queue!" };
        }

        return *this;
    }

    Queue& Queue::submit(CommandBuffer& command_buffer,
                         Semaphore& wait,
                         VkPipelineStageFlags wait_stage,
                         Semaphore& signal,
                         Fence& fence) {
        VkSubmitInfo submit_info {  };
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = nullptr;

        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &wait.get_handle();

        submit_info.pWaitDstStageMask = &wait_stage;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer.get_handle();

        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &signal.get_handle();

        if (VkResult error = vkQueueSubmit(handle, 1, &submit_info, fence.get_handle())) {
            throw Exception { error, "couldn't submit command buffer to the queue!" };
        }

        return *this;
    }

    Queue& Queue::wait_idle() {
        vkQueueWaitIdle(handle);
        return *this;
    }

    Queue& Queue::present(SwapChain& swap_chain,
                          std::uint32_t indices,
                          Semaphore& wait) {
        VkPresentInfoKHR present_info {  };
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &wait.get_handle();
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &swap_chain.get_handle();
        present_info.pImageIndices = &indices;
        present_info.pResults = nullptr;

        auto state = vkQueuePresentKHR(handle,  &present_info);
        swap_chain.set_state(state); // might be out-of-date...

        return *this;
    }

    Queue& Queue::present(SwapChain& swap_chain,
                          std::uint32_t indices) {
        VkPresentInfoKHR present_info {  };
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 0;
        present_info.pWaitSemaphores = nullptr;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &swap_chain.get_handle();
        present_info.pImageIndices = &indices;
        present_info.pResults = nullptr;

        auto state = vkQueuePresentKHR(handle,  &present_info);
        swap_chain.set_state(state); // might be out-of-date...

        return *this;
    }
}

#include "pch.h"
#include "GlobalQueue.h"

/*----------------
	GlobalQueue
-----------------*/

GlobalQueue::GlobalQueue()
{

}

GlobalQueue::~GlobalQueue()
{

}

void GlobalQueue::Push(JobQueuePtr jobQueue)
{
	_jobQueues.Push(jobQueue);
}

JobQueuePtr GlobalQueue::Pop()
{
	return _jobQueues.Pop();
}

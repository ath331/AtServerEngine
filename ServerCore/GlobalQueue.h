#pragma once

/*----------------
	GlobalQueue
-----------------*/

class GlobalQueue
{
public:
	GlobalQueue();
	~GlobalQueue();

	void					Push(JobQueuePtr jobQueue);
	JobQueuePtr				Pop();

private:
	LockQueue<JobQueuePtr> _jobQueues;
};


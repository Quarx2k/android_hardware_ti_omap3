/*
 * Copyright (C) Texas Instruments - http://www.ti.com/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "Semaphore.h"
#include "ErrorUtils.h"
#include <utils/Log.h>


namespace android {

/**
   @brief Constructor for the semaphore class

   @param none
   @return none
 */
Semaphore::Semaphore()
{
    ///Initialize the semaphore to NULL
    mSemaphore = NULL;
}

/**
   @brief Destructor of the semaphore class

   @param none
   @return none

 */
Semaphore::~Semaphore()
{
    ///Destroy only if the sempahore has been created
    if(mSemaphore)
        {
        sem_destroy(mSemaphore);
        free(mSemaphore);
        }
}


/**
   @brief Create the semaphore with initial count value

   @param count >=0
   @return NO_ERROR On Success
   @return NO_MEMORY If unable to allocate memory for the semaphore
   @return BAD_VALUE If an invalid count value is passed (<0)
   @return One of the android error codes based on semaphore initialization
 */

status_t Semaphore::Create(int count)
{
    ///count cannot be less than zero
    if(count<0)
        {
        return BAD_VALUE;
        }

    ///allocate memory for the semaphore
    mSemaphore = (sem_t*)malloc(sizeof(sem_t)) ;

    ///if memory is unavailable, return error
    if(!mSemaphore)
        {
        return NO_MEMORY;
        }

    ///Initialize the semaphore and return the status
    return ErrorUtils::posixToAndroidError(sem_init(mSemaphore, 0x00, count));

}

/**
   @brief Wait operation

   @param none
   @return BAD_VALUE if the semaphore is not initialized
   @return NO_ERROR On success
   @return One of the android error codes based on semaphore wait operation
 */
status_t Semaphore::Wait()
{
    ///semaphore should have been created first
    if(!mSemaphore)
        {
        return BAD_VALUE;
        }

    ///Wait and return the status after signalling
    return ErrorUtils::posixToAndroidError(sem_wait(mSemaphore));


}


/**
   @brief Signal operation

   @param none
     @return BAD_VALUE if the semaphore is not initialized
     @return NO_ERROR On success
     @return One of the android error codes based on semaphore signal operation
   */

status_t Semaphore::Signal()
{
    ///semaphore should have been created first
    if(!mSemaphore)
        {
        return BAD_VALUE;
        }

    ///Post to the semaphore
    return ErrorUtils::posixToAndroidError(sem_post(mSemaphore));

}

/**
   @brief Current semaphore count

   @param none
   @return Current count value of the semaphore
 */
int Semaphore::Count()
{
    int val;

    ///semaphore should have been created first
    if(!mSemaphore)
        {
        return BAD_VALUE;
        }

    ///get the value of the semaphore
    sem_getvalue(mSemaphore, &val);

    return val;
}

/**
   @brief Wait operation with a timeout

     @param timeoutMicroSecs The timeout period in micro seconds
     @return BAD_VALUE if the semaphore is not initialized
     @return NO_ERROR On success
     @return One of the android error codes based on semaphore wait operation
   */

status_t Semaphore::WaitTimeout(int timeoutMicroSecs)
{
    struct timespec timeSpec;

    ///semaphore should have been created first
    if(!mSemaphore)
        {
        return BAD_VALUE;
        }

    ///setup the timeout values - timeout is specified in seconds and nanoseconds
    timeSpec.tv_sec = (timeoutMicroSecs/1000000);
    timeSpec.tv_nsec = (timeoutMicroSecs - timeSpec.tv_sec*1000000)*1000;

    ///Wait for the timeout or signal and return the result based on whichever event occurred first
    return ErrorUtils::posixToAndroidError(sem_timedwait(mSemaphore, &timeSpec));

}


};



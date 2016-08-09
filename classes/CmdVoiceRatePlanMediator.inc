<?php
require_once('../include/TbsDetail.inc');
require_once('../include/TbsApi.inc');

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 4/12/2016
* TODO: 
*/

abstract class CmdVoiceRatePlanMediator
{

	function toTbsProductDetail(VoiceRatePlan $voiceRatePlan, TbsDetail $tbsDetail){
		// required
		//$tbsDetail->ProductsKey = $voiceRatePlan->getProductskey();// required, int, DB key product the charge detail is applied to
		$tbsDetail->LkDetailsKey  = $voiceRatePlan->getId(); //4;//$voiceRatePlan->getId(); // required, int, DB key pointer to the charge detail definition
	}
}

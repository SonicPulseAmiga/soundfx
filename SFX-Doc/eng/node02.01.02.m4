divert(-1)
define(`__TITLE',`Modulator')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_eng.m4')
include(`header.m4')
include(`footer.m4')
include(`navbar.m4')
divert`'dnl
<html>
    __HEADER
    <body __PAGE_STYLE>
        __NAVBAR
        <!-- Contents -->
        __SECTION_LABEL
        __SECTION_STYLE_BEG
            __SECTION_HEADER(__CHAPTER`'__TITLE)
            <tr bgcolor=__COL_SEC_BACK><td>
                <table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
                    <center>
                        __IMAGE(`fx_mod')
                    </center>
                    <br>
                    <p align="justify">This area is for adjusting modulatable parameter in __SFX. In the first
                    row you set start and end values. The '&lt;-&gt;' button lets you swap both values.<br>
                    Now a few words to the parameter themselfs. SIn ce version 3.4 you can use real units in __SFX.
                    E.g. you want to use Amplify to make something sound twice as loud, then you can use the
                    following parameter variations:
                    <table border="0" cellspacing="1" cellpadding="2" width="100%">
                        <tr bgcolor=__COL_TBL_HEAD><th>example      </th><th>description</th></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>2.0          </td><td>factor</td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>200 %        </td><td>absolute, per cent</td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>2000 %%      </td><td>absolute, per thousand</td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>+ 100 %      </td><td>relative, per cent</td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>+ 1000 %%    </td><td>relative, per thousand</td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>+ 6 db       </td><td>relative, decibel</td></tr>
                    </table>
                    As you can see - there are lot of possibilities. Below the units currently known to __SFX
                    (contact me if you need more) :
                    <table border="0" cellspacing="1" cellpadding="2" width="100%">
                        <tr bgcolor=__COL_TBL_HEAD><th>group                </th><th>description                </th><th>format             </th></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>amplitude            </td><td>factor                     </td><td>value               </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>absolute, per cent         </td><td>value %             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>absolute, per thousand     </td><td>value %%            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>relative, per cent         </td><td>+/- value %         </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>relative, per thousand     </td><td>+/- value %%        </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>relative, decibel          </td><td>+/- value db        </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>absolute, level            </td><td>value lv            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>relative frequency   </td><td>factor                     </td><td>value               </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>absolute, per cent         </td><td>value %             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>absolute, per thousand     </td><td>value %%            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>relative, per cent         </td><td>+/- value %         </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>relative, per thousand     </td><td>+/- value %%        </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>relative, semitones        </td><td>+/- value st        </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>relative, cents            </td><td>+/- value ct        </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>relative, semitones &amp; cents </td><td>+/- value:value st:ct</td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>absolute frequency   </td><td>herz                       </td><td>value hz            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>tone                       </td><td>note -/# oktave  (e.q. C-3, E#2)</td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>relative time        </td><td>factor                     </td><td>time               </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>absolute, per cent         </td><td>value %             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>absolute, per thousand     </td><td>value %%            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>repeats             </td><td>value rpts          </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>absolute time        </td><td>hour                       </td><td>value h             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>minute                     </td><td>value m             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>second                     </td><td>value s             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>millisecond                </td><td>value ms            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>second &amp; millisecond   </td><td>value:value s:ms     </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>minute &amp; second        </td><td>value:value m:s      </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>hour &amp; minute &amp; second</td><td>value:value:value h:m:s </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td colspan="2">... I think you`''ve got the idea             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>samples                    </td><td>value sv            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>movie frames (24 fps)      </td><td>value mf            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>PAL-video frames (25 fps)  </td><td>value pf            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>NTSC-video frames (30 fps) </td><td>value nf            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>proportion            </td><td>factor                     </td><td>value               </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>absolute, per cent         </td><td>value %             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>absolute, per thousand     </td><td>value %%            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>count                </td><td>absolute                   </td><td>value               </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>relative                   </td><td>+/- value           </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>phase/angle          </td><td>factor                     </td><td>value               </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>absolute, per cent         </td><td>value %             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>absolute, per thousand     </td><td>value %%            </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>degree                     </td><td>value °             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>minutes                    </td><td>value '             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>seconds                    </td><td>value "             </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>minutes &amp; seconds      </td><td>value:value ':"     </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td colspan="2">... and so on                               </td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td>&nbsp;               </td><td>radian                     </td><td>value rad           </td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td>&nbsp;               </td><td>english degree             </td><td>value grd           </td></tr>
                    </table>
                    Not all of these units could be used for all parameter and otherwise sometimes
                    you can use a unit which is unusual for that parameter. The latter case is mentioned
                    in the the apparent description of the operator.
                    </p>
                    <align="justify">The second row : When coding __SFX I wanted to make it as variable (flexible) as possible. The
                    user should be able to access and edit all the parameters in a way either as simple or complex as he/she desires.
                    This led to the development of the &#39;Blend Shapes&#39;. These are curves (or graphs) that modulate a parameter.
                    A &#39;Blend Shape&#39; always returns values ranging 0.0 - 1.0. This way it can vary a parameter from its start to it
                    end value. The start value gets used at modulation=0.0 and the end value at modulation=1.0.
                    The following variations are implemented :
                    <table border="0" cellspacing="1" cellpadding="2" width="100%">
                        <tr bgcolor=__COL_TBL_HEAD><th>variant              </th><th>description</th></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td valign="top">none    </td><td>This shape returns in every case a value of 0.0 (if you don`t
                                                                                want to modulate something). If you use this enter the value in
                                                                                the first field - the second will be ignored.</td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td valign="top"><a href="node01.07.01.html">curve</a>   </td><td>bended course</td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td valign="top"><a href="node01.07.02.html">cycle</a>   </td><td>oscillation</td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td valign="top"><a href="node01.07.03.html">vector</a>  </td><td>envelope</td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td valign="top"><a href="node01.07.04.html">user</a>    </td><td>user defined</td></tr>
                    </table>
                    </p>
                    <align="justify">Examples say more than thousand words. Here are a few for the Amplify-operator :
                    <table border="0" cellspacing="1" cellpadding="2" width="100%">
                        <tr bgcolor=__COL_TBL_HEAD><th>example          </th><th>description</th></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td valign="top">1   </td><td>You'd like to amplify the volume of the sample by 5%.<br>
                                                                            Par.0 : 105 % (100%+5%)<br>
                                                                            Par.1 : doesn't matter<br>
                                                                            Mode  : None</td></tr>
                        <tr bgcolor=__COL_TBL_ROW2><td valign="top">2   </td><td>You want to amplify the sample to 10% at the start and lower to 60% in
                                                                            the end AND the volume change should accelerate to the end.<br>
                                                                            Par.0 : 110 % (100%+10%)<br>
                                                                            Par.1 :  60<br>
                                                                            Mode  : Curve, Exp="2.0"</td></tr>
                        <tr bgcolor=__COL_TBL_ROW1><td valign="top">3   </td><td>You'd like to produce a tremolo effect (cyclic change of volume -
                                                                            "Helicopter" effect).<br>
                                                                            Par.0 : 120 %<br>
                                                                            Par.1 :  80 %<br>
                                                                            Mode  : Cycle, Sin, Frequency, Frq="1 Hz"</td></tr>
                    </table>
                    </p>
                </td></tr></table>
            </td></tr>
        __SECTION_STYLE_END
        <br>
        __NAVBAR
        __FOOTER
    </body>
</html>

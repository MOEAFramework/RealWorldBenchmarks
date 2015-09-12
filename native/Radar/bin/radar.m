function [o,c]=radar(v)

pri=round(v*1000)+500;  % build correct PRI set values from chromosome
                        % PRI=1/PRF of course.

o=testpris(pri);        % calculate metrics

o=[-o(1:8) o(9)-50];    % minimize objectives 1-8
c=[];

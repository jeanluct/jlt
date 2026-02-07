function load_data(basename)
%LOAD_DATA   Load Matlab data from .mat or .m file.
%   LOAD_DATA(BASENAME) loads data from either BASENAME.mat or
%   BASENAME.m, whichever exists and is most recent. This is useful
%   when working with code that can write either binary .mat files
%   (when compiled with JLT_MATLAB_LIB_SUPPORT) or text .m files.
%
%   The function automatically determines which file to load:
%   - If only one format exists, it loads that one
%   - If both exist, it loads the most recently modified one
%   - If neither exists, it throws an error
%
%   Example:
%       load_data('results')  % Loads results.mat or results.m
%

%
% Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

matfile = [basename '.mat'];
mfile = [basename '.m'];

mat_exists = exist(matfile, 'file') == 2;
m_exists = exist(mfile, 'file') == 2;

if ~mat_exists && ~m_exists
  error(['Neither ' matfile ' nor ' mfile ' exists.']);
end

if mat_exists && ~m_exists
  fprintf('Loading %s\n', matfile);
  load(matfile);
  return
end

if m_exists && ~mat_exists
  fprintf('Loading %s\n', mfile);
  run(mfile);
  return
end

% Both files exist - load the most recent one
mat_info = dir(matfile);
m_info = dir(mfile);

if mat_info.datenum > m_info.datenum
  fprintf('Loading %s (more recent than %s)\n', matfile, mfile);
  load(matfile);
else
  fprintf('Loading %s (more recent than %s)\n', mfile, matfile);
  run(mfile);
end
